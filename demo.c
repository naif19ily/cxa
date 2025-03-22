#include "argxs.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv)
{
    static struct argxs_flag flags[] =
    {
        {"document", 'd', ARGXS_ARG_YES},

        ARGXS_FINAL_FLAG
    };

    struct argxs_res *args = argxs_parse(argc, argv, flags);

    return 0;
}
