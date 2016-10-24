#include <stdio.h>  // for printf
#include <signal.h> // for signal
#include <unistd.h> // for kill, getpid

static void
sighandler(int signum) {
        // fail();
        printf ("\n Signal %d received", signum);

        // Generate core dump
        signal(signum, SIG_DFL);
        kill(getpid(), signum);
}

void
setup_test_runner()
{
        signal(SIGSEGV, sighandler);
}
