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
#include <stdlib.h>

#define TERM_OPT_UNSEEN 0
#define TERM_OPT_SEEN   1

static unsigned char __term_opt_is = TERM_OPT_UNSEEN;


static void* gotta_realloc (const unsigned int, unsigned int*, void**, const size_t);

struct argxs_parsed *argxs (const int argc, char **argv, const struct argxs_flag *flags)
{
    struct argxs_parsed *ps = (struct argxs_parsed*) calloc(1, sizeof(struct argxs_parsed));

    unsigned int cap0 = 16, cap1 = 16;

    ps->flagseen = (struct argxs_seen*) calloc(cap0, sizeof(struct argxs_seen));
    ps->posargs  = (char**) calloc(cap1, sizeof(char*));
    ps->no_parg  = 0;
    ps->no_seen  = 0;
    ps->fatal    = argxs_fatal_none;

    for (int i = 1; i < argc; i++)
    {
        /* if `--` was seen it means anything that comes
         * after must be trataed as a positional argument
         */
        if (__term_opt_is != TERM_OPT_SEEN)
        {
            ps->posargs = (char**) gotta_realloc(ps->no_parg, &cap1, (void*) &ps->posargs, sizeof(*ps->posargs));
            ps->posargs[ps->no_parg++] = argv[i];
            continue;
        }
    }

    return ps;
}

void argxs_clean (struct argxs_parsed *ps)
{
    free(ps);
}

static void* gotta_realloc (const unsigned int ln, unsigned int *cp, void **pt, const size_t sz)
{
    if (ln < *cp) return *pt;

    *cp *= 2;
    *pt = realloc(*pt, sz * *cp);

    if (*pt == NULL) { abort(); }
    return *pt;
}
