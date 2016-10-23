#include <signal.h> // for signal, SIG_INT

#include "common/error.h"
#include "common/macros.h"

static int exitGracefully = 0;

static void
sigHandler(int sig)
{
        UNUSED(sig);
        exitGracefully = 1;
}

void
setup_signals()
{
        if (signal(SIGINT, sigHandler) == SIG_ERR) {
                errExit("signal");
        }
}

int
keep_looping()
{
        return exitGracefully == 0;
}
