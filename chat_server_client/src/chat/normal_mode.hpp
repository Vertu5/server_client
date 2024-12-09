#pragma once
#include "chat_mode.hpp"
#include <mutex>
#include <sys/socket.h>

class NormalMode : public ChatMode {
public:
    NormalMode(const ProgramOptions& opts);
    void runSenderThread(int sockfd) override;
    void runReceiverThread(int sockfd) override;

protected:
    ProgramOptions opts;
    std::mutex print_mutex; 
    void displayMessage(const std::string& from, const std::string& message, bool isBot);
};
