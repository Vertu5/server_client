#include "bot_mode.hpp"
#include <iostream>
#include <unistd.h>

void BotMode::runSenderThread(int sockfd) {
    std::string line;
    while (true) {
        if(!std::cin.good()) break;
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

        // En mode bot, on n'affiche pas les messages envoyés
    }

    shutdown(sockfd, SHUT_WR);
}
