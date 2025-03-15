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

struct argxs_flag
{
    char    *name;
    int32_t id;
    uint8_t q_arg;
};

int32_t argxs_get (const int32_t, char**, const struct argxs_flag const*);

#endif
