#include "signals.hpp"
#include <csignal>
#include <cstddef>

// Pour ce client, on va surtout gérer SIGPIPE. Le SIGINT sera géré en manuel_mode si nécessaire.
static void client_signal_handler(int sig) {
    // SIGPIPE ignoré, SIGINT géré ailleurs en manuel
}

void setupSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = client_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGPIPE, &sa, NULL);
}
