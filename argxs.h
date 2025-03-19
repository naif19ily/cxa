/*
 *                 __
 *                / _)
 *       _.----._/ /      dc0x13
 *      /         /       part of `argxs` project.
 *   __/ (  | (  |        Mar 18 2025
 *  /__.-'|_|--|_|
 */
#ifndef ARGXS_ARGXS_H
#define ARGXS_ARGXS_H

#define ARGXS_FLAGS_ARG_IS_NONE 0
#define ARGXS_FLAGS_ARG_IS_OPTN 1
#define ARGXS_FLAGS_ARG_IS_NEED 2

#define ARGXS_FINAL_FLAG        {NULL, 0, 0}

enum argxs_fatals
{
    argxs_fatal_none = 0,
};

struct argxs_flag
{
    const char          *name;
    const char          id;
    const unsigned char q_arg;
};

struct argxs_flag_seen
{
    struct argxs_flag *flag;
    char              *arg;
};

struct argxs_parsed
{
    struct argxs_flag_seen *flagseen;
    char                   **posargs;
    unsigned short         noflagseen;
    unsigned short         noposargs;
    enum argxs_fatals      fatal;
};

struct argxs_parsed *argxs (const int, char**, const struct argxs_flag*);

#endif
