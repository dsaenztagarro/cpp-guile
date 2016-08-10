#include <stdio.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#pragma clang diagnostic ignored "-Wlong-long"

#include <libguile.h>

#pragma clang diagnostic pop


static void* register_functions(void *data)
{
        return NULL;
}

int main ()
{
        SCM func;

        scm_init_guile();

        scm_c_primitive_load( "src/script.scm" );

        func = scm_variable_ref( scm_c_lookup( "simple-script" ) );

        scm_call_0( func );

        return 0;
}
