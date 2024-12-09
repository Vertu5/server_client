#include "normal_mode.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>

NormalMode::NormalMode(const ProgramOptions& options) : opts(options) {}

void NormalMode::runSenderThread(int sockfd) {
    std::string line;
    while (true) {
        if(!std::cin.good()) break;
        if(!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) continue;

        size_t space_pos = line.find(' ');
        if (space_pos == std::string::npos) {
            // format invalide
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
    }

    shutdown(sockfd, SHUT_WR);
}

void NormalMode::runReceiverThread(int sockfd) {
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

        size_t rb = received.find(']');
        if (rb == std::string::npos) continue;
        std::string from = received.substr(1, rb-1);
        std::string msg = received.substr(rb+2);

        {
            std::lock_guard<std::mutex> lock(print_mutex);
            displayMessage(from, msg, opts.isBot);
        }
    }
}

void NormalMode::displayMessage(const std::string& from, const std::string& message, bool isBot) {
    if (opts.isJoli) {
        // Option "joli" : indentation si reçu de quelqu'un d'autre
        if (from != opts.user) {
            std::cout << std::string(40, ' ');
        }
    }

    if (isBot) {
        std::cout << "[" << from << "] " << message << "\n";
    } else {
        std::cout << "\x1B[4m[" << from << "]\x1B[0m " << message << "\n";
    }
    std::cout.flush();
}
