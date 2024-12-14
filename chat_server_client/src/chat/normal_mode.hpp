/**
 * @file normal_mode.hpp 
 * @brief Définition du mode normal du chat (utilisateur)
 */

#pragma once
#include "chat_mode.hpp"
#include <mutex>
#include <sys/socket.h>

class NormalMode : public ChatMode {
public:
    /**
     * @brief constructeur
     * @param opts Configuration du programme
     */
    NormalMode(const ProgramOptions& opts);
    void runSenderThread(int sockfd) override;
    void runReceiverThread(int sockfd) override;

protected:
    ProgramOptions opts;
    // initialisation d'un mutex
    std::mutex print_mutex; 
    void displayMessage(const std::string& from, const std::string& message, bool isBot);
};
