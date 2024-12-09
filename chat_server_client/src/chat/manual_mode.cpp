#include "manual_mode.hpp"
#include <iostream>
#include <csignal>
#include <cstring>
#include <unistd.h>

std::atomic<bool> ManualMode::g_sigintReceived(false);

void ManualMode::manual_signal_handler(int sig) {
    if (sig == SIGINT) {
        g_sigintReceived.store(true);
    }
}

ManualMode::ManualMode(const ProgramOptions& options) : NormalMode(options) {
    struct sigaction sa;
    sa.sa_handler = ManualMode::manual_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}

void ManualMode::runSenderThread(int sockfd) {
    std::string line;
    while (true) {
        if (g_sigintReceived.load()) {
            displayPendingMessages();
            g_sigintReceived.store(false);
        }

        if (!std::cin.good()) break;
        if(!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) continue;

        size_t space_pos = line.find(' ');
        if (space_pos == std::string::npos) {
            continue;
        }
        std::string dest = line.substr(0, space_pos);
        std::string msg = line.substr(space_pos + 1);

        std::string to_send = dest + " " + msg;
        ssize_t s = send(sockfd, to_send.c_str(), to_send.size(), 0);
        if (s <= 0) {
            break;
        }

        if (!opts.isBot) {
            std::lock_guard<std::mutex> lock(print_mutex);
            displayMessage(opts.user, msg, false);
        }

        displayPendingMessages();
    }

    shutdown(sockfd, SHUT_WR);
}

void ManualMode::runReceiverThread(int sockfd) {
    char buffer[2048];
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t r = recv(sockfd, buffer, sizeof(buffer)-1, 0);
        if (r <= 0) {
            break;
        }

        std::string received = buffer;
        if (received.find("n'est pas connectée.") != std::string::npos) {
            std::cerr << received;
            continue;
        }

        std::cout << '\a' << std::flush;
        size_t rb = received.find(']');
        if (rb == std::string::npos) continue;
        std::string from = received.substr(1, rb-1);
        std::string msg = received.substr(rb+2);

        std::string final_msg;
        if (opts.isBot) {
            final_msg = "[" + from + "] " + msg;
        } else {
            final_msg = "\x1B[4m[" + from + "]\x1B[0m " + msg;
        }

        addMessage(final_msg);
    }
}

void ManualMode::addMessage(const std::string& msg) {
    std::lock_guard<std::mutex> lock(msg_mutex);
    pendingMessages.push_back(msg);
    totalBytes += msg.size();
    if (totalBytes >= 4096) {
        displayPendingMessages();
    }
}

void ManualMode::displayPendingMessages() {
    std::lock_guard<std::mutex> lock(msg_mutex);
    for (auto& m : pendingMessages) {
        std::lock_guard<std::mutex> plock(print_mutex);
        std::cout << m << "\n";
    }
    pendingMessages.clear();
    totalBytes = 0;
}
