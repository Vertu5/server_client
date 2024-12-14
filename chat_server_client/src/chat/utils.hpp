/**
 * @file utils.hpp
 * @brief Définition de fonctions nécessaire au bon fonctionnement du chat
 */

#pragma once
#include <string>
#include <iostream>
#include <iomanip>

inline bool validatePseudo(const std::string& pseudo) {
    // verifie si le pseudo a bien une longueur inférieur à 30 charachtère
    if (pseudo.length() > 30) return false;

    // verifie les pseudo spécial
    if (pseudo == "." || pseudo == "..") return false;

    // verifie les charactère invalides
    const std::string invalidChars = "/-[]";
    for (char c : pseudo) {
        if (invalidChars.find(c) != std::string::npos) {
            return false;
        }
    }
    return true;
}

ProgramOptions parseArgs(int argc, char* argv[]) {
    // Check minimal number of arguments
    if (argc < 2) {
        std::cerr << "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]" << std::endl;
        exit(MISSING_ARGS); // Code 1
    }

    ProgramOptions opts;
    opts.user = argv[1];
    // opts.dest = argv[2];

    // Check pseudonyms length
    if (opts.user.length() > MAX_PSEUDO_LENGTH) { //|| opts.dest.length() > MAX_PSEUDO_LENGTH) {
        std::cerr << "Erreur : pseudo trop long (max " << MAX_PSEUDO_LENGTH << " caractères)" << std::endl;
        exit(PSEUDO_TOO_LONG); // Code 2
    }

    // Check for invalid characters and invalid pseudos
    if (!validatePseudo(opts.user)) { //  || !validatePseudo(opts.dest)) {
        exit(INVALID_CHARS); // Code 3
    }

    // Process options --bot and --manuel
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--bot") {
            opts.isBot = true;  // Correction ici
        }
        else if (arg == "--manuel") {
            opts.isManual = true;  // Correction ici
        }
        else if (arg == "--joli") {
            opts.isJoli = true;
        }
        else {
            // Unknown parameter encountered
            std::cerr << "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]" << std::endl;
            exit(MISSING_ARGS); // Code 1
        }
    }

    return opts;
}

// trouver un endroit ou l appeler
void displayWelcome(const ProgramOptions& opts) {
    if (opts.isJoli) {
        std::string mode = "normal + joli";
        if (opts.isBot && opts.isManual) mode = "bot + manuel + joli";
        else if (opts.isBot) mode = "bot + joli";
        else if (opts.isManual) mode = "manuel + joli";

        std::cout << "\n╔════════════════════════════╗" << std::endl;
        std::cout << "║      Chat démarré          ║" << std::endl;
        std::cout << "╠════════════════════════════╣" << std::endl;
        std::cout << "║ Utilisateur : " << std::left << std::setw(13) << opts.user << "║" << std::endl;
        // std::cout << "║ Destinataire : " << std::left << std::setw(12) << opts.dest << "║" << std::endl;
        std::cout << "║ Mode : " << std::left << std::setw(20) << mode << "║" << std::endl;
        std::cout << "╠════════════════════════════╣" << std::endl;
        std::cout << "║ 'exit' pour quitter        ║" << std::endl;
        std::cout << "╚════════════════════════════╝" << std::endl << std::endl;
    }
}