#include "argxs.h"
#include <stdio.h>

int main (int argc, char **argv)
{
    static const struct argxs_flag flags[] =
    {
        {"doc", 'D', ARGXS_FLAGS_ARG_IS_OPTN},
        ARGXS_FINAL_FLAG
    };

    const struct argxs_parsed *ps = argxs(argc, argv, flags);
    printf("error: %s\n", argxs_why_fatal[ps->fatal]);

    return 0;
}
