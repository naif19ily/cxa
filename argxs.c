/*
 *                 __
 *                / _)
 *       _.----._/ /
 *      /         /     argxs
 *   __/ (  | (  |
 *  /__.-'|_|--|_|
 */

#include "argxs.h"

int main (void)
{
    struct argxs_flag flags[] = {
        {"document", 'D', ARGXS_FLAG_ARG_NEED},
        ARGXS_FINISHER_FLAG,
    };
    return 0;
}
