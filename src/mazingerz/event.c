#include <stdio.h>     // for fprintf
#include <stdlib.h>    // for exit
#include <sys/event.h> // for kqueue, kevent
#include <sys/file.h>  // for O_RDONLY
#include <unistd.h>    // for close

void
create_event(struct kevent *kev, int ident, void *udata)
{
        // EVFILT_VNODE Takes a file descriptor as the identifier and the
        // events to watch for in fflags, and returns when one or more of the
        // requested events occurs on the descriptor.
        int filter = EVFILT_VNODE;

        // Actions to perform on the event
        unsigned int flags = EV_ADD | EV_ENABLE | EV_CLEAR;

        // Filter-specific flags
        unsigned int fflags = NOTE_WRITE;

        EV_SET (kev, ident, filter, flags, fflags, 0, udata);
}

void
register_event(int kq, char *filelist[], int nfiles)
{
        struct kevent changelist[nfiles];

        for (int i = 0; i < nfiles; i++) {
                int fd;
                char *filename = filelist[i];

                if ((fd = open(filename, O_RDONLY, 0)) == -1) {
                        fprintf(stderr, "Error: Failed to open file %s\n", filename);
                        // TODO: return error code
                        exit(-1);
                }

                struct kevent *change = changelist + i;
                create_event(change, fd, filename);
        }

        kevent(kq, changelist, nfiles, NULL, 0, NULL);
}

void
event_loop(int kq)
{
        while (1) {
                // camp on kevent() until something interesting happens
                struct kevent change;
                if (kevent(kq, NULL, 0, &change, 1, NULL) == -1) { exit(1); }
                // The signal event has NULL in the user data.  Check for that first.
                if (change.udata == NULL) {
                        fprintf (stderr, "Error: Received event without user data");
                        exit(-1);
                } else {
                        if (change.fflags == NOTE_WRITE) {
                                printf ("Write %s\n", (char*)change.udata);
                                break;
                        }
                }
        }
        close(kq);
}
