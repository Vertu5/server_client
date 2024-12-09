#include "chat.hpp"
#include "utils.hpp"
#include "signals.hpp"
#include "chat_mode.hpp"
#include "normal_mode.hpp"
#include "bot_mode.hpp"
#include "manual_mode.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring> // pour memset

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "chat pseudo_utilisateur [--bot] [--manuel] [--joli]\n";
        return MISSING_ARGS;
    }

    ProgramOptions opts;
    opts.user = argv[1];
    if (!validatePseudo(opts.user)) {
        std::cerr << "Pseudo invalide.\n";
        return INVALID_CHARS;
    }

    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--bot") {
            opts.isBot = true;
        } else if (arg == "--manuel") {
            opts.isManual = true;
        } else if (arg == "--joli") {
            opts.isJoli = true;
        }
    }

    std::string ip = "127.0.0.1";
    const char* ip_env = std::getenv("IP_SERVEUR");
    if (ip_env) {
        ip = ip_env;
    }

    int port = 1234;
    const char* port_env = std::getenv("PORT_SERVEUR");
    if (port_env) {
        int p = std::atoi(port_env);
        if (p > 0 && p <= 65535) {
            port = p;
        }
    }

    setupSignalHandlers();

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur création socket\n";
        return SYSTEM_ERROR;
    }

    struct sockaddr_in serv_addr;

    std::memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Adresse IP invalide\n";
        return SYSTEM_ERROR;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Impossible de se connecter au serveur.\n";
        return SYSTEM_ERROR;
    }

    // Envoyer pseudo
    {
        ssize_t s = send(sockfd, opts.user.c_str(), opts.user.size(), 0);
        if (s <= 0) {
            std::cerr << "Erreur envoi pseudo\n";
            close(sockfd);
            return SYSTEM_ERROR;
        }
    }

    std::unique_ptr<ChatMode> mode;
    if (opts.isManual) {
        mode = std::make_unique<ManualMode>(opts);
    } else if (opts.isBot) {
        mode = std::make_unique<BotMode>(opts);
    } else {
        mode = std::make_unique<NormalMode>(opts);
    }

    std::thread sender(&ChatMode::runSenderThread, mode.get(), sockfd);
    std::thread receiver(&ChatMode::runReceiverThread, mode.get(), sockfd);

    sender.join();
    receiver.join();

    close(sockfd);
    return SUCCESS;
}
