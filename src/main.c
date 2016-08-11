#include <stdio.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#pragma clang diagnostic ignored "-Wlong-long"

#include <libguile.h>

#pragma clang diagnostic pop

#include <dirent.h>


int main ()
{
        DIR* tasks_dir;
        struct dirent* task_file;

        if (NULL == (tasks_dir = opendir("examples/tasks")))
        {
                fprintf(stderr, "Error: Failed to open tasks directory\n");
        }

        while ((task_file = readdir(tasks_dir)))
        {
                /* On linux/Unix we don't want current and parent directories
                 * If you're on Windows machine remove this two lines
                 */
                if (!strcmp (task_file->d_name, "."))
                        continue;
                if (!strcmp (task_file->d_name, ".."))
                        continue;
                fprintf(stdout, "%s", task_file->d_name);
        }

        SCM func;

        scm_init_guile();

        scm_c_primitive_load( "examples/tasks/rspec.scm" );

        func = scm_variable_ref( scm_c_lookup( "rspec" ) );

        char *filepath = "/tmp/file.rb";

        scm_call_1(func, scm_from_utf8_string(filepath));

        return 0;
}
