#pragma once
#include <string>
#include <atomic>

class Serveur;

// Structure pour stocker infos clients
struct ClientInfo {
    int sockfd;
    std::string pseudo;
};

class ClientHandler {
public:
    ClientHandler(int sockfd, Serveur* server);
    void run();

private:
    int client_sock;
    Serveur* server;
    std::string pseudo;

    bool handshake();
    void mainLoop();
    bool processMessage(const std::string& ligne);
};
