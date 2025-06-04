#include "cxa.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv)
{
    static struct cxa_flag flags[] =
    {
        {"document", 'd', ARGXS_ARG_YES},
        {"depth",    'D', ARGXS_ARG_MAY},
        {"help",     'H', ARGXS_ARG_NON},
        ARGXS_FINAL_FLAG
    };

    struct cxa_res *res = cxa_parse(argc, argv, flags);

    if (res->fatal != cxa_fatal_none)
    {
        printf("error: %s\n", cxa_errors[res->fatal]);
        switch (res->fatal)
        {
            case cxa_fatal_non_sense:
            case cxa_fatal_undef_flag:
                printf("located at %d within argv: %s\n", res->argc, argv[res->argc]);
                break;
            case cxa_fatal_unnecessary_arg:
                printf("extra argument located at %d within argv: %s\n", res->argc, argv[res->argc]);
                printf("flag: %s\n", res->last->flag->flagname);
                break;
            case cxa_fatal_arg_expected:
                printf("missing argument located at %d within argv: %s\n", res->argc, argv[res->argc]);
                printf("flag: %s\n", res->last->flag->flagname);
                break;
        }
        return 0;
    }

    puts("flags:");
    for (unsigned int i = 0; i < res->no_found; i++)
    {
        const struct cxa_found *f = &res->found[i];
        printf("<%s> := <%s>\n", f->flag->flagname, f->argument);
    }

    puts("\narguments:");
    for (unsigned int i = 0; i < res->no_p_args; i++)
    {
        printf(" - %s\n", res->p_args[i]);
    }

    cxa_clean(res);
    return 0;
}
