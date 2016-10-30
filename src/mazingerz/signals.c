#include <signal.h> // for signal, SIG_INT

#include "common/error.h"
#include "common/macros.h"

static int exit_gracefully = 0;

void stop_execution();

static void
sighandler(int sig)
{
        UNUSED(sig);
        stop_execution();
}

void
catch_signals()
{
        if (signal(SIGINT, sighandler) == SIG_ERR)
                errExit("signal");
}

int
continue_execution()
{
        return exit_gracefully == 0;
}

void
stop_execution()
{
        exit_gracefully = 1;
}
