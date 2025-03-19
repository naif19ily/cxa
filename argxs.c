#include "argxs.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERM_OPT_UNSEEN     0
#define TERM_OPT_SEEN       1

static unsigned char __term_opt_is  = TERM_OPT_UNSEEN;
static unsigned int *__flag_lengths = NULL;

enum argv_kind
{
    argvs_error,
    argvs_long_flag,
    argvs_shrt_flag,
    argvs_argument,
    argvs_endopmkr,
};

static void calculate_flagname_lengths (const struct argxs_flag*);
static void* gotta_realloc (const unsigned int, unsigned int*, void**, const size_t);

static enum argv_kind what_is_this (const char*);
static enum argxs_fatals long_flag (const struct argxs_flag*, struct argxs_seen*, const char*);

struct argxs_parsed *argxs (const int argc, char **argv, const struct argxs_flag *flags)
{
    calculate_flagname_lengths(flags);

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
        if (__term_opt_is == TERM_OPT_SEEN)
        {
            ps->posargs = (char**) gotta_realloc(ps->no_parg, &cap1, (void*) &ps->posargs, sizeof(*ps->posargs));
            ps->posargs[ps->no_parg++] = argv[i];
            printf(">> argument found\n");
            continue;
        }

        switch (what_is_this(argv[i]))
        {
            case argvs_long_flag:
            case argvs_shrt_flag:
            {
                printf("flag found\n");
                break;
            }

            case argvs_argument:
            {
                printf("argument found\n");
                break;
            }

            case argvs_endopmkr:
            {
                printf("end found\n");
                __term_opt_is = TERM_OPT_SEEN;
                break;
            }

            default:
            {
                printf("error found\n");
                break;
            }
        }
    }

    return ps;
}

void argxs_clean (struct argxs_parsed *ps)
{
    free(ps);
}

static void calculate_flagname_lengths (const struct argxs_flag *flags)
{
    unsigned int nflags = 0;
    for (; flags[nflags].name != NULL; nflags++) ;;

    __flag_lengths = (unsigned int*) calloc(nflags, sizeof(unsigned int));
    for (unsigned int i = 0; i < nflags; i++)
    {
        __flag_lengths[i] = (unsigned int) strlen(flags[i].name);
    }
}

static void* gotta_realloc (const unsigned int ln, unsigned int *cp, void **pt, const size_t sz)
{
    if (ln < *cp) return *pt;

    *cp *= 2;
    *pt = realloc(*pt, sz * *cp);

    if (*pt == NULL) { abort(); }
    return *pt;
}

static enum argv_kind what_is_this (const char *thing)
{
    if ((*thing == '-') && (thing[1] == '-'))
    {
        if (thing[2] == '\0') return argvs_endopmkr;
        if (isalnum(thing[2])) return argvs_long_flag;
        return argvs_error;
    }

    if ((*thing == '-'))
    {
        return isalnum(thing[1]) ? argvs_shrt_flag : argvs_error;
    }

    return argvs_argument;
}

static enum argxs_fatals long_flag (const struct argxs_flag *flags, struct argxs_seen *seen, const char *name)
{
    // 1. get until you find a = (if any)
    // 2. compare this length against the precomputed already

    unsigned int eqat = 0;

    for (int i = 0; flags[i].name != NULL; i++)
    {
        if (!strncmp(NULL, NULL, 0))
        {
            seen->flag = (struct argxs_flag*) &flags[i];
            seen->arg = NULL;
            return argxs_fatal_none;
        }
    }

    return argxs_fatal_unknown_flag;
}
