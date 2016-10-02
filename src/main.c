#include <stdio.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#pragma clang diagnostic ignored "-Wlong-long"

#include <libguile.h>

#pragma clang diagnostic pop

#include <dirent.h>    // for DIR, dirent
#include <fcntl.h>     // for open
#include <sys/event.h> // for kqueue, kevent

#include "vendor/libguile"

int main ()
{
        int nfiles = 2;
        char *filelist[] = { "fixtures/test1.txt", "fixtures/test2.txt" };

        int kq = kqueue();

        register_event(kq, filelist, nfiles);

        event_loop(kq);

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
