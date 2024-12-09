#include "client_handler.hpp"
#include "serveur.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>

ClientHandler::ClientHandler(int sockfd, Serveur* srv) : client_sock(sockfd), server(srv) {}

bool ClientHandler::handshake() {
    char pseudo_buf[64];
    std::memset(pseudo_buf, 0, sizeof(pseudo_buf));
    ssize_t r = recv(client_sock, pseudo_buf, sizeof(pseudo_buf)-1, 0);
    if (r <= 0) {
        return false;
    }

    pseudo = pseudo_buf;
    if (pseudo.empty() || !validatePseudo(pseudo)) {
        return false;
    }

    if (!server->registerClient(pseudo, client_sock)) {
        return false;
    }

    return true;
}

void ClientHandler::run() {
    if (!handshake()) {
        close(client_sock);
        return;
    }

    mainLoop();

    server->unregisterClient(pseudo);
    close(client_sock);
}

void ClientHandler::mainLoop() {
    char buffer[2048];
    while (server->isRunning()) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t len = recv(client_sock, buffer, sizeof(buffer)-1, 0);
        if (len <= 0) {
            // Déconnexion
            break;
        }

        std::string ligne = buffer;
        if (!processMessage(ligne)) {
            break; // Déconnexion si message trop long
        }
    }
}

bool ClientHandler::processMessage(const std::string& ligne) {
    // Format : "destinataire message"
    size_t space_pos = ligne.find(' ');
    if (space_pos == std::string::npos) {
        // format invalide, on ignore
        return true;
    }
    std::string dest = ligne.substr(0, space_pos);
    std::string msg = ligne.substr(space_pos + 1);

    if (msg.size() > 1024) {
        // Message trop long -> déconnecter ce client
        server->disconnectClient(pseudo);
        return false;
    }

    // Envoyer le message au destinataire
    server->sendMessageToClient(dest, pseudo, msg);
    return true;
}
