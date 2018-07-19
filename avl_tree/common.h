#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <stddef.h>
#include <errno.h>

#define prln() printf("\n FILE %s; LINE %i\n", __FILE__, __LINE__)
#define prhello() printf("\n\033[1m\033[31m\033[4m\033[5m" \
                         "I am here! Line №%i" \
                         "\033[0m\n", __LINE__);

#define ALLOC(a)        \
do {                    \
    if (!(a))           \
    {                   \
        return -ENOMEM; \
    }                   \
} while (0)

/**
  * Ways of going tree elements over
  */
enum
{
    MODE_PREFIX = 0,
    MODE_INFIX,
    MODE_POSTFIX,
};

/**
  * Constants to tell if the node left of right
  */
enum
{
    AVLN_DFLT = -1,
    AVLN_RGHT = 0,
    AVLN_LEFT = 1,
};

#endif
