/**
 * @file manual_mode.hpp
 * @brief Définition du mode manuel du chat 
 * 
 */

#pragma once
#include "normal_mode.hpp"
#include <vector>
#include <atomic>
#include <mutex>

class ManualMode : public NormalMode {
public:
    /**
     * @brief constructeur
     * @param opts Configuration du programme
     */
    ManualMode(const ProgramOptions& opts);

    void runSenderThread(int sockfd) override;
    void runReceiverThread(int sockfd) override;

private:
    // initialisation d'un mutex
    std::mutex msg_mutex;
    std::vector<std::string> pendingMessages;
    size_t totalBytes = 0;
    // initialisation d'un booléen afin de savoir si le signal SIGINT a été reçu
    static std::atomic<bool> g_sigintReceived;

    // initialisation du handler pour lesignal SIGINT
    static void manual_signal_handler(int sig);

    void addMessage(const std::string& msg);
    void displayPendingMessages();
};
