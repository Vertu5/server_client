#include "serveur.hpp"
#include "utils.hpp"
#include "client_handler.hpp"
#include <iostream>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <thread>

static std::atomic<bool> g_terminate(false);

void Serveur::handleSignals(int sig) {
    if (sig == SIGINT) {
        g_terminate.store(true);
    } else if (sig == SIGPIPE) {
        // On peut ignorer
    }
}

Serveur::Serveur() : server_fd(-1), port(1234), running(true) {
    loadPortFromEnv();
    setupSignalHandlers();
}

Serveur::~Serveur() {
    if (server_fd >= 0) close(server_fd);
}

void Serveur::loadPortFromEnv() {
    const char* env_p = std::getenv("PORT_SERVEUR");
    if (env_p) {
        int p = std::atoi(env_p);
        if (p > 0 && p <= 65535) {
            port = p;
        }
    }
}

void Serveur::setupSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = Serveur::handleSignals;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        std::cerr << "Erreur configuration SIGINT\n";
        exit(1);
    }

    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        std::cerr << "Erreur configuration SIGPIPE\n";
        exit(1);
    }
}

void Serveur::run() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Erreur création socket\n";
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Erreur bind\n";
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Erreur listen\n";
        exit(1);
    }

    std::cout << "Serveur démarré sur le port " << port << ". Ctrl+C pour arrêter.\n";

    std::thread accept_thread(&Serveur::acceptClients, this);
    accept_thread.join();

    // Nettoyage des clients
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& kv : clients) {
            close(kv.second);
        }
        clients.clear();
    }

    std::cout << "Serveur arrêté.\n";
}

void Serveur::acceptClients() {
    while (!g_terminate.load()) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
        if (client_sock < 0) {
            if (errno == EINTR) break; 
            if (g_terminate.load()) break;
            std::cerr << "Erreur accept\n";
            continue;
        }

        std::thread t([this, client_sock]() {
            ClientHandler handler(client_sock, this);
            handler.run();
        });
        t.detach();
    }

    stopServer();
}

void Serveur::stopServer() {
    running.store(false);
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
    server_fd = -1;
}

bool Serveur::registerClient(const std::string& pseudo, int sock) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    if (clients.find(pseudo) != clients.end()) {
        return false;
    }
    clients[pseudo] = sock;
    return true;
}

void Serveur::unregisterClient(const std::string& pseudo) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(pseudo);
}

void Serveur::sendMessageToClient(const std::string& dest, const std::string& from, const std::string& content) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = clients.find(dest);
    if (it == clients.end()) {
        // Dest pas connecté
        auto sender = clients.find(from);
        if (sender != clients.end()) {
            sendErrorNotConnected(sender->second, dest);
        }
        return;
    }

    std::string final_msg = "[" + from + "] " + content;
    send(it->second, final_msg.c_str(), final_msg.size(), 0);
}

void Serveur::sendErrorNotConnected(int sock, const std::string& dest) {
    std::string err = "Cette personne (" + dest + ") n'est pas connectée.\n";
    send(sock, err.c_str(), err.size(), 0);
}

void Serveur::disconnectClient(const std::string& pseudo) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = clients.find(pseudo);
    if (it != clients.end()) {
        close(it->second);
        clients.erase(it);
    }
}
