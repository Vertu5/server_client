#pragma once
#include <string>
#include <atomic>

const int MAX_PSEUDO_LENGTH = 30;

enum ReturnCodes {
    SUCCESS = 0,
    MISSING_ARGS = 1,
    PSEUDO_TOO_LONG = 2,
    INVALID_CHARS = 3,
    SIGINT_EXIT = 4,
    SYSTEM_ERROR = 5,
    RESOURCE_ERROR = 7,
    DISCONNECTED = 8,
    STDIN_CLOSED = 9
};

struct ProgramOptions {
    std::string user;
    bool isBot = false;
    bool isManual = false; 
    bool isJoli = false;
};
