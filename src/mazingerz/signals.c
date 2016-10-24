#include <signal.h> // for signal, SIG_INT

#include "common/error.h"
#include "common/macros.h"

static int exit_gracefully = 0;

static void
sighandler(int sig)
{
        UNUSED(sig);
        exit_gracefully = 1;
}

void
catch_signals()
{
        if (signal(SIGINT, sighandler) == SIG_ERR)
                errExit("signal");
}

int
not_interrupted()
{
        return exit_gracefully == 0;
}
