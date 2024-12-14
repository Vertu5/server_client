/**
 * @file client_handler.hpp
 * @brief definition du struct ClientInfo ainsi que de la classe ClientHandler
 */

#pragma once
#include <string>
#include <atomic>

class Serveur;

// Structure pour stocker infos clients
struct ClientInfo {
    int sockfd;
    std::string pseudo;
};

/** 
 * @brief gère la communication entre un client connecté avec le serveur
 */
class ClientHandler {
public:
    /** 
     * @brief constructeur
     * @param sockfd socket utilisé
     * @param server serveur au quelle le client va se connecté
     */
    ClientHandler(int sockfd, Serveur* server);
    void run();

private:
    int client_sock;
    Serveur* server;
    std::string pseudo;

    // verifie si le destinataire est bien connecté au serveur
    bool handshake();
    void mainLoop();
    bool processMessage(const std::string& ligne);
};
