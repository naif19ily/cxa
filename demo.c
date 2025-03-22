#include "argxs.h"
#include <stdio.h>

int main (int argc, char **argv)
{
    static const struct argxs_flag flags[] =
    {
        {"document", 'D', ARGXS_FLAGS_ARG_IS_NEED},
        {"depth",    'd', ARGXS_FLAGS_ARG_IS_NEED},
        {"page",     'p', ARGXS_FLAGS_ARG_IS_OPTN},
        {"help",     'h', ARGXS_FLAGS_ARG_IS_NONE},
        ARGXS_FINAL_FLAG
    };

    struct argxs_parsed *ps = argxs(argc, argv, flags);

    if (ps->fatal != argxs_fatal_none)
    {
        printf("error: %s (argv at %d) (`%s` flag)\n", argxs_why_fatal[ps->fatal], ps->err_at, argxs_last_flag ? argxs_last_flag->flag->name : "none");
        return 0;
    }

    char* argument = NULL;
    for (unsigned int i = 0; i < ps->no_seen; i++)
    {
        printf("seen: `%s` flag with argument: `%s`\n", ps->flagseen[i].flag->name, ps->flagseen[i].arg);
        argument = ps->flagseen[i].arg;
    }

    return 0;
}
