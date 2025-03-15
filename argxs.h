/*
 *                 __
 *                / _)
 *       _.----._/ /
 *      /         /     argxs
 *   __/ (  | (  |
 *  /__.-'|_|--|_|
 */
#ifndef ARGXS_ARGXS_H
#define ARGXS_ARGXS_H

#ifndef uint8_t
# define uint8_t unsigned char
#endif

#ifndef uint16_t
# define uint16_t unsigned short
#endif

#ifndef uint32_t
# define uint32_t unsigned int
#endif

#ifndef uint64_t
# define uint64_5 unsigned long
#endif

#ifndef int8_t
# define int8_t signed char
#endif

#ifndef int16_t
# define int16_t signed short
#endif

#ifndef int32_t
# define int32_t signed int
#endif

#ifndef int64_t
# define int64_5 signed long
#endif

#ifndef bool
# define bool unsigned char
#endif

#ifndef true
# define true 1
#endif

#ifndef false
# define false 0
#endif

/* Used to indicate if a flag does take
 * argument.
 * NT = does not take.
 * YT = does take.
 * MT = might take.
 */
#define ARGXS_FLAG_ARG_NONE 0
#define ARGXS_FLAG_ARG_NEED 1
#define ARGXS_FLAG_ARG_MAYB 2

/* This must always be included as the last
 * flag in the `flags` array
 */
#define ARGXS_FINISHER_FLAG {NULL, 0, 0}

/* An argument is made of a name such as `document` and id
 * to be indentified with, for example 'D' which stands for
 * document (IDs can only be [a-zA-Z0-9] since other characters
 * are interpreted as errors, and finally and argument option
 * which denotes if the flag does take an argument or not.
 */
struct argxs_flag
{
    char    *name;
    char    id;
    uint8_t q_arg;
};

struct argxs_seen
{
    char     *its_arg;
    char     id;
    bool     seen;
};

enum argxs_error_type
{
    err_no_error          = 0,
    err_bad_id_definition = 1,
    err_flag_isnt_defined = 2,
    err_no_argument_needd = 3,
    err_arg_without_flag  = 4,
    err_unknown_shit      = 5,
};

/* Reason why the parser failed.
 */
extern enum argxs_error_type argxs_fatal_reason = err_no_error;

/* Points to the current flag in case there is an error
 * any information can be obtained from here.
 */
extern struct argxs_flag *argxs_current_flag;

/* Points to the last element read within argv, useful
 * for error handling.
 */
extern char *argxs_current_element_in_argv;

struct argxs_seen* argxs_get (const int32_t, char**, struct argxs_flag *const);

#endif
