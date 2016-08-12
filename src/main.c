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

int main ()
{
        int kq = kqueue ();

        struct kevent direvent;

        int dirfd = open("examples/tasks", O_RDONLY, 0);

        unsigned int vnode_events;

        vnode_events = NOTE_DELETE |  NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE;
        EV_SET (&direvent, dirfd, EVFILT_VNODE, EV_ADD | EV_CLEAR | EV_ENABLE,
                vnode_events, 0, 0);

        kevent(kq, &direvent, 1, NULL, 0, NULL);

        while (1) {
                // camp on kevent() until something interesting happens
                struct kevent change;
                if (kevent(kq, NULL, 0, &change, 1, NULL) == -1) { exit(1); }
                // The signal event has NULL in the user data.  Check for that first.
                if (change.udata == NULL) {
                        break;
                } else {
                        // udata is non-null, so it's the name of the directory
                        printf ("%s\n", (char*)change.udata);
                }
        }
        close (kq);


        DIR* tasks_dir;
        struct dirent* task_file;

        if (NULL == (tasks_dir = opendir("examples/tasks")))
        {
                fprintf(stderr, "Error: Failed to open tasks directory\n");
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
