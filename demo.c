/*
 *                 __
 *                / _)
 *       _.----._/ /
 *      /         /     argxs
 *   __/ (  | (  |
 *  /__.-'|_|--|_|
 */

#include "argxs.h"
#include <stdio.h>

// exe --document <filename> or exe -D <filename> or exe --document=filename

int main (int argc, char **argv)
{
    struct argxs_flag flags[] = {
        {"document", 'D', ARGXS_FLAG_ARG_NEED},
        ARGXS_FINISHER_FLAG,
    };

    (void) argxs_get(argc, argv, flags);
    return 0;
}
