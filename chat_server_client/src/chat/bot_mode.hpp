/**
 * @file bot_mode.hpp 
 * @brief Définition de la classe pour le mode bot du chat
 */

#pragma once
#include "normal_mode.hpp"

/**
 * @brief Classe gérant le mode bot du chat
 * 
 * @details Cette classe hérite de NormalMode et modifie le comportement 
 * d'affichage pour :
 * - Désactiver le formatage du texte (pas de soulignement) 
 * - Ne pas afficher les messages envoyés par l'utilisateur
 * Le mode bot est activé avec l'option --bot
*/
class BotMode : public NormalMode {
public:
    BotMode(const ProgramOptions& opts) : NormalMode(opts) {}
    void runSenderThread(int sockfd) override;
};
