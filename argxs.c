#include "argxs.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Attemps to classify the type of the current
 * element within argv
 */
enum argvs_kind
{
    argvs_non_sense,
    argvs_argument,
    argvs_long_flag,
    argvs_shrt_flag,
    argvs_term_flag,
};

/* Whenever `--` is found, it means there is not more flags, whatever
 * comes next must be trated as an optional argument
 */
static unsigned char __op_args_only = 0;

/* Stores the lengths of all flags in order to avoid re-computation
 * when `handle_long_flag` is called
 */
static unsigned int *__flag_lengths = NULL;

static struct argxs_res *make_res (void);
static void compute_lengths (const struct argxs_flag*);

static enum argvs_kind determinate_kind (const char*);
static enum argxs_fatal handle_long_flag (const char*, const struct argxs_flag*, struct argxs_found*);

struct argxs_res *argxs_parse (const unsigned int argc, char **argv, const struct argxs_flag *flags)
{
    struct argxs_res *res = make_res();
    compute_lengths(flags);

    unsigned int found_cap = 16, pargs_cap = 16;

    for (; (res->argc < argc) && (res->fatal == argxs_fatal_none); res->argc++)
    {
        if (__op_args_only == 1)
        {
            puts("optional argument");
            continue;
        }

        const char *_argv = argv[res->argc];
        switch (determinate_kind(_argv))
        {
            case argvs_non_sense:
                res->fatal = argxs_fatal_non_sense;
                break;

            case argvs_long_flag:
                res->fatal = handle_long_flag(_argv + 2, flags, &res->found[res->no_found++]);
                break;

            case argvs_shrt_flag: puts("shrt-flag"); break;
            case argvs_term_flag: puts("term-flag"); __op_args_only = 1; break;
            case argvs_argument : puts("argument"); break;
        }
    }

    return res;
}

static struct argxs_res *make_res (void)
{
    struct argxs_res *res = (struct argxs_res*) calloc(1, sizeof(struct argxs_res));

    res->p_args    = (char**) calloc(16, sizeof(char**));
    res->found     = (struct argxs_foun**) calloc(16, sizeof(struct argxs_found));
    res->last      = NULL;
    res->argc      = 1;
    res->no_found  = 0;
    res->no_p_args = 0;
    res->fatal     = argxs_fatal_none;

    // TODO: make sure alloc was successful

    return res;
}

static void compute_lengths (const struct argxs_flag *flags)
{
    unsigned int nf = 0;
    while (flags[nf].flagname != NULL) nf++;

    __flag_lengths = (unsigned int*) calloc(nf, sizeof(unsigned int));
    for (unsigned int i = 0; i < nf; i++)
    {
        __flag_lengths[i] = (unsigned int) strlen(flags[i].flagname);
    }

    // TODO: make sure alloc was successful
}

static enum argvs_kind determinate_kind (const char *argv)
{
    if ((*argv == '-') && (argv[1] == '-'))
    {
        if (argv[2] == '\0') return argvs_term_flag;
        return isalnum(argv[2]) ? argvs_long_flag : argvs_non_sense;
    }
    if (*argv == '-')
    {
        return isalnum(argv[1]) ? argvs_shrt_flag : argvs_non_sense;
    }
    return argvs_argument;
}

static enum argxs_fatal handle_long_flag (const char *argv, const struct argxs_flag *flags, struct argxs_found *_flag)
{
    unsigned int eqs = 0, len = 0;
    for (; argv[len] != '\0'; len++) { if (argv[len] == '=') { eqs = len; } }


    const unsigned int thslen = (eqs == 0) ? len : eqs;
    struct argxs_flag *flag   = NULL;
    unsigned char found       = 0;

    for (unsigned int i = 0; (flags[i].flagname != NULL) && (found == 0); i++)
    {
        flag = (struct argxs_flag*) &flags[i];
        const unsigned int n = __flag_lengths[i] > thslen ? __flag_lengths[i] : thslen;
        if (strncmp(argv, flag->flagname, n) == 0) { found = 1; }
    }

    if (found == 0) return argxs_undef_flag;

    _flag->flag     = flag;
    _flag->argument = (eqs == 0) ? NULL : argv + eqs + 1;
    return argxs_fatal_none;
}

// -D 3456765
// --document=345
// --document 4543


