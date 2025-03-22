#ifndef ARGXS_ARGXS_H
#define ARGXS_ARGXS_H

/* Defines if a flag does take an argument, for example
 * if the -d flag is defined as ARGXS_ARG_YES, then its
 * usage must be -d "argument", else argxs will detect
 * it as an error
 */
#define ARGXS_ARG_NON 0
#define ARGXS_ARG_YES 1
#define ARGXS_ARG_MAY 2

/* This must be always included within the array of flags
 * since this works as a the delimiter
 */
#define ARGXS_FINAL_FLAG    {NULL, 0, 0}

enum argxs_fatal
{
    argxs_fatal_none
};

/* flagname: a long name for the flag (--document)
 * id:       simple character representation (-D)
 * q_arg:    argument option
 */
struct argxs_flag
{
    char          *flagname;
    char          id;
    unsigned char q_arg;
};

/* flag:      pointer to the original flag
 * argument:  given argument (if any)
 * allocated: was the argument allocated into the heap?
 */
struct argxs_found
{
    struct        argxs_flag *flag;
    char          *argument;
    unsigned char allocated;
};

/* p_args:    positional arguments
 * found:     found flags
 * last:      last flag used (in case of error)
 * argc:      position within argv
 * no_found:  number of flags found
 * no_p_args: number of positional arguments
 * fatal:     cause of error (if any)
 */
struct argxs_res
{
    char         **p_args;
    struct       argxs_found **found;
    struct       argxs_flag *last;
    unsigned int argc;
    unsigned int no_found;
    unsigned int no_p_args;
    enum         argxs_fatal fatal;
};

struct argxs_res *argxs_parse (const int, char**, const struct argxs_flag*);

#endif
