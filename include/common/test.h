#define assert(test, message) do {\
        if (!(test)) {\
                printf("%s failed:\n\t%s:%d: %s", __func__, __FILE__, __LINE__, message);\
        }\
} while (0)

#define mu_run_test(test) do { char *message = test(); tests_run++; \
        if (message) return message; } while (0)
        extern int tests_run;
