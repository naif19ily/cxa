#ifndef ARGXS_ARGXS_H
#define ARGXS_ARGXS_H

#define ARGXS_FLAGS_ARG_IS_NONE 0
#define ARGXS_FLAGS_ARG_IS_OPTN 1
#define ARGXS_FLAGS_ARG_IS_NEED 2

#define ARGXS_CLEAN_ALL         0
#define ARGXS_CLEAN_STRUCT_ONLY 1

#define ARGXS_FINAL_FLAG        {NULL, 0, 0}

enum argxs_fatals
{
    argxs_fatal_none = 0,
    argxs_fatal_unknown_flag,
    argxs_fatal_malformed_flag,
    argxs_fatal_unnecessary_arg,
    argxs_fatal_missed_argument
};

struct argxs_flag
{
    const char          *name;
    const char          id;
    const unsigned char q_arg;
};

struct argxs_seen
{
    struct argxs_flag *flag;
    char              *arg;
    unsigned char     arg_can_be_freed;
};

struct argxs_parsed
{
    struct argxs_seen *flagseen;
    char              **posargs;
    unsigned int      no_seen;
    unsigned int      no_parg;
    unsigned int      err_at;
    enum argxs_fatals fatal;
};

/* Message errros, the programmer can use them if they want
 * to (i guess).
 */
extern const char *const argxs_why_fatal[5];

/* Last flag seen, this is useful for error messages.
 */
extern struct argxs_seen *argxs_last_flag;

struct argxs_parsed *argxs (const int, char**, const struct argxs_flag*);
void argxs_clean (struct argxs_parsed*, const unsigned char);

#endif
