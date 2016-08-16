#include <stdio.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#pragma clang diagnostic ignored "-Wlong-long"

#include <libguile.h>

#pragma clang diagnostic pop

#include <dirent.h>    // for DIR, dirent
#include <fcntl.h>     // for open
#include <sys/file.h>  // for O_RDONLY
#include <sys/event.h> // for kqueue, kevent


void new_file_event(struct kevent *kev, int ident, void *udata)
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

void register_file_events(int kq, char *filelist[], int nfiles)
{
        struct kevent changelist[nfiles];

        for (int i = 0; i < nfiles; i++) {
                int fd;
                char *filename = filelist[i];

                if ((fd = open(filename, O_RDONLY, 0)) == -1) {
                        fprintf(stderr, "Error: Failed to open file %s\n", filename);
                        exit(-1);
                }

                struct kevent *change = changelist + i;
                new_file_event(change, fd, filename);
        }

        kevent(kq, changelist, nfiles, NULL, 0, NULL);
}

void listen_file_events(int kq)
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
        close (kq);
}

int main ()
{
        int nfiles = 2;
        char *filelist[] = { "fixtures/test1.txt", "fixtures/test2.txt" };

        int kq = kqueue();

        register_file_events(kq, filelist, nfiles);

        listen_file_events(kq);


        DIR* tasks_dir;
        struct dirent* task_file;

        if (NULL == (tasks_dir = opendir("examples/taskss")))
        {
                fprintf(stderr, "Error: Failed to open tasks directory\n");
                exit(-1);
        }

        while ((task_file = readdir(tasks_dir)))
        {
                if (!strcmp (task_file->d_name, "."))
                        continue;
                if (!strcmp (task_file->d_name, ".."))
                        continue;
                fprintf(stdout, "%s\n", task_file->d_name);
        }

        SCM func;

        scm_init_guile();

        scm_c_primitive_load( "examples/tasks/rspec.scm" );

        func = scm_variable_ref( scm_c_lookup( "rspec" ) );

        char *filepath = "/tmp/file.rb";

        scm_call_1(func, scm_from_utf8_string(filepath));

        return 0;
}
