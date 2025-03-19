#include "argxs.h"

#include <err.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERM_OPT_UNSEEN     0
#define TERM_OPT_SEEN       1

#define MAX_OF(a, b)        ((a) > (b) ? (a) : (b))

/* Indicates if `--` flag was found, once it is found anything that
 * comes next is trated as a positional argument
 */
static unsigned char __term_opt_is  = TERM_OPT_UNSEEN;

/* Since everytime a --flagname is found the program needs to compare
 * what is found against the defined flags, it needs to calculate those
 * name lengths, which causes redundant calculations, this saves the length
 * of every flag defined.
 */
static unsigned int *__flag_lengths = NULL;

static struct argxs_seen *__last_seen = NULL;

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

static void get_unix_like_argument (const char*, char**);
static enum argxs_fatals shrt_flag (const struct argxs_flag*, struct argxs_seen*, const char);

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

    for (int i = 1; (i < argc) && (ps->fatal != argxs_fatal_none); i++)
    {
        ps->err_at = i;

        if (__term_opt_is == TERM_OPT_SEEN)
        {
            ps->posargs = (char**) gotta_realloc(ps->no_parg, &cap1, (void*) &ps->posargs, sizeof(*ps->posargs));
            ps->posargs[ps->no_parg++] = argv[i];
            continue;
        }

        switch (what_is_this(argv[i]))
        {
            case argvs_long_flag:
            {
                gotta_realloc(ps->no_seen, &cap0, (void*) &ps->flagseen, sizeof(*ps->flagseen));
                ps->fatal = long_flag(flags, &ps->flagseen[ps->no_seen++], argv[i] + 2);
                break;
            }

            case argvs_shrt_flag:
            {
                gotta_realloc(ps->no_seen, &cap0, (void*) &ps->flagseen, sizeof(*ps->flagseen));
                ps->fatal = shrt_flag(flags, &ps->flagseen[ps->no_seen++], argv[i][1]);
                break;
            }

            case argvs_argument:
            {
                if (__last_seen->flag->q_arg != ARGXS_FLAGS_ARG_IS_NONE && __last_seen->arg != NULL)
                {
                    __last_seen->arg = argv[i];
                }
                else
                {
                    ps->fatal = argxs_fatal_unnecessary_arg;
                }
                break;
            }

            case argvs_endopmkr:
            {
                __term_opt_is = TERM_OPT_SEEN;
                break;
            }

            default:
            {
                ps->fatal = argxs_fatal_malformed_flag;
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

    if (*pt == NULL) { err(EXIT_FAILURE, "cannot realloc"); }
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
        if (thing[2] != '\0') return argvs_error;
        return isalnum(thing[1]) ? argvs_shrt_flag : argvs_error;
    }

    return argvs_argument;
}

static enum argxs_fatals long_flag (const struct argxs_flag *flags, struct argxs_seen *seen, const char *thing)
{
    const unsigned int thglen = (unsigned int) strlen(thing);
    unsigned int eqat = 0;

    for (unsigned int i = 0; i < thglen; i++)
    {
        if (thglen && thing[i] == '=')
        {
            if (!isalnum(thing[i + 1])) { return argxs_fatal_malformed_flag; }
            eqat = i;
            break;
        }
    }

    const unsigned int nc = (eqat == 0) ? thglen : eqat;
    for (int i = 0; flags[i].name != NULL; i++)
    {
        if (!strncmp(flags[i].name, thing, MAX_OF(nc, __flag_lengths[i])))
        {
            if (flags[i].q_arg == ARGXS_FLAGS_ARG_IS_NONE) { return argxs_fatal_unknown_flag; }

            seen->flag = (struct argxs_flag*) &flags[i];
            seen->arg  = NULL;
            if (eqat != 0) { get_unix_like_argument(thing + eqat + 1, &seen->arg); }

            __last_seen = seen;
            return argxs_fatal_none;
        }
    }
    return argxs_fatal_unknown_flag;
}

static void get_unix_like_argument (const char *seto, char **dest)
{
    const unsigned int length = (unsigned int) strlen(seto);
    *dest = (char*) calloc(length + 1, sizeof(char));

    if (*dest == NULL) { err(EXIT_FAILURE, "cannot alloc"); }
    snprintf(*dest, length + 1, "%s", seto);
}

static enum argxs_fatals shrt_flag (const struct argxs_flag *flags, struct argxs_seen *seen, const char id)
{
    for (int i = 0; flags[i].name != NULL; i++)
    {
        if (flags[i].id == id)
        {
            seen->flag = (struct argxs_flag*) &flags[i];
            seen->arg  = NULL;

            __last_seen = seen;
            return argxs_fatal_none;
        }
    }

    return argxs_fatal_unknown_flag;
}
