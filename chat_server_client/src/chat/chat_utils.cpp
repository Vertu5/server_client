#include "chat.hpp"
#include <sys/stat.h>
#include <iomanip>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>

namespace Chat {

bool validatePseudo(const std::string& pseudo) {
    // Check length (max 30 characters)
    if (pseudo.length() > MAX_PSEUDO_LENGTH) {
        std::cerr << "Erreur : pseudo trop long (max " << MAX_PSEUDO_LENGTH << " caractères)" << std::endl;
        return false;
    }

    // Check for special pseudos
    if (pseudo == "." || pseudo == "..") {
        std::cerr << "Erreur : pseudo invalide" << std::endl;
        return false;
    }

    // Check for invalid characters
    const std::string invalidChars = "/-[]";
    if (pseudo.find_first_of(invalidChars) != std::string::npos) {
        std::cerr << "Erreur : caractères invalides dans le pseudo" << std::endl;
        return false;
    }

    return true;
}

ProgramOptions parseArgs(int argc, char* argv[]) {
    // Check minimal number of arguments
    if (argc < 3) {
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
    // if (!validatePseudo(opts.user) || !validatePseudo(opts.dest)) {
    //     exit(INVALID_CHARS); // Code 3
    // }

    // Process options --bot and --manuel
    for (int i = 3; i < argc; i++) {
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

void notifyMessageReceived() {
    std::cout << '\a' << std::flush;
}

}

