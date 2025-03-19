/*
 *                 __
 *                / _)
 *       _.----._/ /      dc0x13
 *      /         /       part of `argxs` project.
 *   __/ (  | (  |        Mar 18 2025
 *  /__.-'|_|--|_|
 */
#include "argxs.h"
#include <stdio.h>

int main (int argc, char **argv)
{
    static const struct argxs_flag flags[] =
    {
        {"document", 'D', ARGXS_FLAGS_ARG_IS_NEED},
        ARGXS_FINAL_FLAG
    };
    return 0;
}
