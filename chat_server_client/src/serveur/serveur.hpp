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

    void run();
    bool registerClient(const std::string& pseudo, int sock);
    void unregisterClient(const std::string& pseudo);
    void sendMessageToClient(const std::string& dest, const std::string& from, const std::string& content);
    void sendErrorNotConnected(int sock, const std::string& dest);
    void disconnectClient(const std::string& pseudo);

    bool isRunning() const { return running.load(); }

private:
    int server_fd; 
    int port;
    std::atomic<bool> running;
    std::mutex clients_mutex;

    std::map<std::string, int> clients; // pseudo -> socket

    void loadPortFromEnv();
    void setupSignalHandlers();
    void acceptClients();
    static void handleSignals(int sig);

    void stopServer();
};
