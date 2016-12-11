#define assert(message, test) do {\
        if (test) {\
                printf("%s::%s %s PASSED\n", __FILE__, __func__, message);\
        } else {\
                printf("%s::%s %s FAILED\n", __FILE__, __func__, message);\
        }\
} while (0)

#define mu_run_test(test) do { char *message = test(); tests_run++; \
        if (message) return message; } while (0)
        extern int tests_run;

void
setup_test_runner();
