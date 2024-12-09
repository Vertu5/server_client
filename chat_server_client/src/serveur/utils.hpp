#pragma once
#include <string>

inline bool validatePseudo(const std::string& pseudo) {
    if (pseudo == "." || pseudo == "..") return false;
    const std::string invalidChars = "/-[]";
    if (pseudo.size() > 30) return false;
    for (char c : pseudo) {
        if (invalidChars.find(c) != std::string::npos)
            return false;
    }
    return true;
}
