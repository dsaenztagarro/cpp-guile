#include <stdio.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#pragma clang diagnostic ignored "-Wlong-long"

#include <libguile.h>

#pragma clang diagnostic pop


int main ()
{
        SCM func;

        scm_init_guile();

        scm_c_primitive_load( "examples/tasks/rspec.scm" );

        func = scm_variable_ref( scm_c_lookup( "rspec-task" ) );

        char *filepath = "/tmp/file.rb";

        scm_call_1(func, scm_from_utf8_string(filepath));

        return 0;
}
