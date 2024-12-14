/**
 * @file chat_mode.hpp 
 * @brief Définition de la classe ChatMode
 * 
 * C'est la généralisation de la classe NormalMode
 */

#pragma once
#include "chat.hpp"
#include <string>

class ChatMode {
public:
    virtual ~ChatMode() = default;
    virtual void runSenderThread(int sockfd) = 0;
    virtual void runReceiverThread(int sockfd) = 0;
};
