#include "argxs.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

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

static unsigned char __op_args_only = 0;

static struct argxs_res *make_res (void);
static enum argvs_kind determinate_kind (const char*);

struct argxs_res *argxs_parse (const int argc, char **argv, const struct argxs_flag *flags)
{
    struct argxs_res *res = make_res();
    unsigned int found_cap = 16, pargs_cap = 16;

    for (; (res->argc < argc) && (res->fatal == argxs_fatal_none); res->argc++)
    {
        if (__op_args_only == 1)
        {
            puts("optional argument");
            continue;
        }

        switch (determinate_kind(argv[res->argc]))
        {
            case argvs_non_sense: puts("non-sense"); break;
            case argvs_long_flag: puts("long-flag"); break;
            case argvs_shrt_flag: puts("shrt-flag"); break;
            case argvs_term_flag: puts("term-flag"); __op_args_only = 1; break;
        }
    }

    return res;
}

static struct argxs_res *make_res (void)
{
    struct argxs_res *res = (struct argxs_res*) calloc(1, sizeof(struct argxs_res));

    res->p_args    = (char**) calloc(16, sizeof(char**));
    res->found     = (struct argxs_found**) calloc(16, sizeof(struct argxs_found*));
    res->last      = NULL;
    res->argc      = 1;
    res->no_found  = 0;
    res->no_p_args = 0;
    res->fatal     = argxs_fatal_none;

    return res;
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

// -D 3456765
// --document=345
// --document 4543
