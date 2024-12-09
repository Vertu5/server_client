#pragma once
#include "normal_mode.hpp"
#include <vector>
#include <atomic>
#include <mutex>

class ManualMode : public NormalMode {
public:
    ManualMode(const ProgramOptions& opts);

    void runSenderThread(int sockfd) override;
    void runReceiverThread(int sockfd) override;

private:
    std::mutex msg_mutex;
    std::vector<std::string> pendingMessages;
    size_t totalBytes = 0;
    static std::atomic<bool> g_sigintReceived;

    static void manual_signal_handler(int sig);

    void addMessage(const std::string& msg);
    void displayPendingMessages();
};
