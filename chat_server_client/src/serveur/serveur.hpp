/**
 * @file serveur.hpp
 * @brief intialise la classe Serveur
 */

#pragma once
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <netinet/in.h>

struct ClientInfo;

class Serveur {
public:
    Serveur();
    ~Serveur();

    // point d'entrée principal pour démarrer le serveur
    void run();
    // enregistre un client avec un pseudo unique
    bool registerClient(const std::string& pseudo, int sock);
    // supprime un client de la liste des clients enregistrés
    void unregisterClient(const std::string& pseudo);
    // envoie un message à un client spécifique
    void sendMessageToClient(const std::string& dest, const std::string& from, const std::string& content);
    // notifie un client que le destinataire n'est pas connecté
    void sendErrorNotConnected(int sock, const std::string& dest);
    // déconnecte un client et ferme sa connexion
    void disconnectClient(const std::string& pseudo);

    bool isRunning() const { return running.load(); }

private:
    int server_fd; 
    int port;
    std::atomic<bool> running;
    std::mutex clients_mutex;

    std::map<std::string, int> clients; // pseudo -> socket

    // charge le port du serveur à partir de la variable d'environnement "PORT_SERVEUR"
    void loadPortFromEnv();
    void setupSignalHandlers();
    void acceptClients();
    static void handleSignals(int sig);

    void stopServer();
};
