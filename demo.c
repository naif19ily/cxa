#include "argxs.h"
#include <stdio.h>

int main (int argc, char **argv)
{
    static const struct argxs_flag flags[] =
    {
        {"doc", 'D', ARGXS_FLAGS_ARG_IS_NEED},
        ARGXS_FINAL_FLAG
    };

    argxs(argc, argv, flags);
    return 0;
}
