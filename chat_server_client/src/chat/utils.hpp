#pragma once
#include <string>

inline bool validatePseudo(const std::string& pseudo) {
    if (pseudo.length() > 30) return false;
    if (pseudo == "." || pseudo == "..") return false;
    const std::string invalidChars = "/-[]";
    for (char c : pseudo) {
        if (invalidChars.find(c) != std::string::npos) {
            return false;
        }
    }
    return true;
}
