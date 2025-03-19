/*
 *                 __
 *                / _)
 *       _.----._/ /      dc0x13
 *      /         /       part of `argxs` project.
 *   __/ (  | (  |        Mar 18 2025
 *  /__.-'|_|--|_|
 */
#include "argxs.h"

#define TERM_OPT_UNSEEN 0
#define TERM_OPT_SEEN   1
#define GROWTH_FACTOR   16

static unsigned char __term_opt_is = TERM_OPT_UNSEEN;

struct argxs_parsed *argxs (const int argc, char **argv, const struct argxs_flag *flags)
{
    struct argxs_parsed *ps;
    ps->flagseen   = (struct argxs_flag_seen*) calloc(GROWTH_FACTOR, sizeof(struct argxs_flag_seen));
    ps->posargs    = (char**) calloc(GROWTH_FACTOR, sizeof(*char));
    ps->noflagseen = 0;
    ps->noposargs  = 0;
    ps->fatal      = argxs_fatal_none;

    for (int i = 1; i < argc; i++)
    {
    }
}
