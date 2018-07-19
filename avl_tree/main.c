#include "avln.h"
#include "avlt.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#undef UT_DEBUG
#undef CALLOC_CHECK

//------------------------------------------------------

int test_insert(void);
int test_delete(void);
int test_dump(void);
int test_find(void);
int test_running_low(void);
int test_for_each(void);
int test_avln(void);
int test_calloc(void);

#define PERFORM_TEST(NAME)                \
do {                                      \
    if (test_##NAME())                    \
    {                                     \
        printf("test "#NAME" failed\n");  \
        exit(1);                          \
    }                                     \
} while (0)

void unit_test(void)
{
    PERFORM_TEST(insert);
    PERFORM_TEST(delete);
    PERFORM_TEST(dump);
    PERFORM_TEST(find);
    PERFORM_TEST(running_low);
    PERFORM_TEST(for_each);
    PERFORM_TEST(avln);
#ifdef CALLOC_CHECK
    PERFORM_TEST(calloc);
#endif
}

#undef PERFORM_TEST

//------------------------------------------------------

#ifdef UT_DEBUG
#   define DPRINTF(...) printf(__VA_ARGS__)
#   define DUMP(tree)   avlt_dump(tree)
#else
#   define DPRINTF(...)
#   define DUMP(...)
#endif

int test_insert(void)
{
    struct avlt* t = avlt_ctor(0);
    for (int i = 30; i > 15; i--)
    {
        int ins_ret = avlt_insert(t, i);
        DPRINTF("insert %d; result %2d\n", i, ins_ret);
        if (((i == 0) && (ins_ret != EEXIST)) ||
            ((i != 0) && (ins_ret != 0))      )
        {
            return 1;
        }
    }
    int ins_arr[16] = {5, 10, 15, 4, 6, 13, 12, 2, 11, 1, 0, 3, 8, 7, 9, 14};
    for (int i = 0; i < 16; i++)
    {
        int ins_ret = avlt_insert(t, ins_arr[i]);
        DPRINTF("insert %d; result %2d\n", ins_arr[i], ins_ret);
        if (((ins_arr[i] == 0) && (ins_ret != EEXIST)) ||
            ((ins_arr[i] != 0) && (ins_ret != 0))      )
        {
            return 1;
        }
    }
    avlt_dtor(t);

    return 0;
}

int test_delete(void)
{
    struct avlt* t = avlt_ctor(0);
    for (int i = 0; i < 15; i++)
    {
        int ins_ret = avlt_insert(t, i);
        if (((i == 0) && (ins_ret != EEXIST)) ||
            ((i != 0) && (ins_ret != 0))      )
        {
            return 1;
        }
        DPRINTF("insert %d; result %2d\n", i, ins_ret);
    }
    DUMP(t);

    int del_arr[16] = {5, 10, 15, 4, 6, 13, 12, 2, 11, 1, 0, 3, 8, 7, 9, 14};
    for (int i = 0; i < 16; i++)
    {
        int del_ret = avlt_delete(t, del_arr[i]);
        if (((i == 2) && (del_ret != ENOKEY)) ||
            ((i != 2) && (del_ret != 0))      )
        {
            return 1;
        }
        DPRINTF("delete %d; result %2d; next %2d\n",
                 del_arr[i], del_ret, del_arr[(i + 1) % 15]);
        DUMP(t);
    }
    if (t->root != 0)
    {
        return 1;
    }

    avlt_dtor(t);
    return 0;
}

int test_dump(void)
{
    struct avlt* t = avlt_ctor(0);
    for (int i = 1; i < 3; i++)
    {
        avlt_insert(t, i);
    }
    avlt_dump(t);
    int dump_fd = open("treeDump001.gv", O_RDONLY);
    if (dump_fd < 0)
        return -1;
    char buf[1 << 16];
    int read_ret = read(dump_fd, buf, 1 << 16);
    if (read_ret < 0)
    {
        close(dump_fd);
        return -1;
    }
    buf[read_ret] = '\0';
    if (strlen(buf) != 1071)
    {
        return -1;
    }
    avlt_dtor(t);
    return 0;
}

int test_find(void)
{
    struct avlt* t = avlt_ctor(0);
    for (int i = 1; i < 16; i++)
    {
        avlt_insert(t, i);
    }
    if (avlt_find(t, 25) != NULL)
    {
        return 1;
    }
    struct avln* found_node = avlt_find(t, 10);
    if ((found_node == NULL)    ||
        (found_node->key != 10))
    {
        return 1;
    }
    avlt_dtor(t);
    return 0;
}

int test_running_low(void)
{
    struct avlt* t = avlt_ctor(0);
    int del_ret = avlt_delete(t, 0);
    if (del_ret != 0)
    {
        return 1;
    }
    int ins_ret = avlt_insert(t, 1);
    if (ins_ret != 0)
    {
        return 1;
    }
    avlt_dtor(t);
    return 0;
}

void avlt_max(struct avlt* t,
              struct avln* n,
                     void* ctx)
{
    int* max = (int*)ctx;
    if (n->key > *max)
    {
        *max = n->key;
    }
}

int test_for_each(void)
{
    struct avlt* t = avlt_ctor(0);
    avlt_insert(t, 3);
    avlt_insert(t, -1);
    avlt_insert(t, 5);

    int max_value;
    int MODES[3] = {MODE_PREFIX, MODE_INFIX, MODE_POSTFIX};
    for (int i = 0; i < 3; i++)
    {
        max_value = -666;
        avlt_for_each(t, MODES[i], &max_value, avlt_max);
        if (max_value != 5)
            return 1;
    }
    avlt_dtor(t);
    return 0;
}

int test_avln(void)
{
    struct avln* n    = avln_ctor(2, NULL, NULL, NULL, AVLN_DFLT);
    struct avln* left = avln_ctor(1, NULL, NULL,    n, 1);
    struct avln* rght = avln_ctor(3, NULL, NULL,    n, 0);

    avln_dtor(n);
    avln_dtor(left);
    avln_dtor(rght);
    return 0;
}

int test_calloc(void)
{
    struct rlimit rls = { .rlim_cur = 64000, .rlim_max = 64000 };
    if (setrlimit(RLIMIT_AS, &rls) != 0)
    {
        perror("setrlimit");
        exit(1);
    }
    struct avlt* t = avlt_ctor(0);
    for (int i = 1; i < 250000; i++)
    {
        int ins_ret = avlt_insert(t, i);
        if (ins_ret == ENOMEM)
        {
            avlt_dtor(t);
            return 0;
        }
    }
    return 1;
}

#undef DPRINTF
#undef DUMP

int main(void)
{
    unit_test();
    printf("All tests have succeeded!\n");
    return 0;
}
