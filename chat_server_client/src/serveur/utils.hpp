/**
 * @file utils.hpp
 * @brief Définition de fonctions nécessaire au bon fonctionnement du chat
 */

#pragma once
#include <string>


inline bool validatePseudo(const std::string& pseudo) {
    // verifie les pseudo spécial
    if (pseudo == "." || pseudo == "..") return false;

    // verifie les charactère invalides
    const std::string invalidChars = "/-[]";
    if (pseudo.size() > 30) return false;
    for (char c : pseudo) {
        if (invalidChars.find(c) != std::string::npos)
            return false;
    }
    return true;
}
