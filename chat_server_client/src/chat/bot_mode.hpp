#pragma once
#include "normal_mode.hpp"

class BotMode : public NormalMode {
public:
    BotMode(const ProgramOptions& opts) : NormalMode(opts) {}
    void runSenderThread(int sockfd) override;
};
