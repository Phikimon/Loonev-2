#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avlt.h"

/* (n->balance_factor == 2) &&
   (n->rght->balance_factor <= 0) */
static void avlt_left_left_rotate(struct avln* n)
{
    assert(n);
    struct avln* b = n->rght;
    avln_new_parent(b,             NULL, n->uppr, avln_is_left(n));
    avln_new_parent(b->left, &(b->left), n,       AVLN_RGHT);
    avln_new_parent(n,             NULL, b,       AVLN_LEFT);
}

/* (n->balance_factor == -2) &&
   (n->left->balance_factor <= 0) */
static void avlt_rght_rght_rotate(struct avln* n)
{
    assert(n);
    struct avln* b = n->left;
    avln_new_parent(b,             NULL, n->uppr, avln_is_left(n));
    avln_new_parent(b->rght, &(b->rght), n,       AVLN_LEFT);
    avln_new_parent(n,             NULL, b,       AVLN_RGHT);
}

/* (n->balance_factor == -2) &&
   (n->left->balance_factor > 0) */
static void avlt_left_rght_rotate(struct avln* n)
{
    assert(n);
    avlt_left_left_rotate(n->left);
    avlt_rght_rght_rotate(n);
}

/* (n->balance_factor == 2) &&
   (n->rght->balance_factor > 0) */
static void avlt_rght_left_rotate(struct avln* n)
{
    assert(n);
    avlt_rght_rght_rotate(n->rght);
    avlt_left_left_rotate(n);
}

static void avlt_balance(struct avln* n)
{
    assert(n);
    if ((n->balance_factor == 2) &&
        (n->rght->balance_factor >= 0))
    {
        avlt_left_left_rotate(n);
    } else
    if ((n->balance_factor == -2) &&
            (n->left->balance_factor <= 0))
    {
        avlt_rght_rght_rotate(n);
    } else
    if ((n->balance_factor == -2) &&
            (n->left->balance_factor > 0))
    {
        avlt_left_rght_rotate(n);
    } else
    if ((n->balance_factor == 2) &&
            (n->rght->balance_factor < 0))
    {
        avlt_rght_left_rotate(n);
    }
}

struct avlt* avlt_ctor(int k)
{
    struct avlt* new_avlt = (struct avlt*)calloc(1,
                             sizeof(struct avlt));
    if (new_avlt == NULL)
    {
        goto out_err;
    }

    new_avlt->root = (struct avln*)calloc(1,
                            sizeof(struct avln));
    if (new_avlt->root == NULL)
    {
        goto free_avlt;
    }
    new_avlt->root->key = k;
    return new_avlt;

free_avlt:
    free(new_avlt);
out_err:
    return NULL;
}

static void avlt_recursive_dtor(struct avln* n)
{
    if (!n) return;
    avlt_recursive_dtor(n->left);
    avlt_recursive_dtor(n->rght);
    avln_dtor(n);
}

void avlt_dtor(struct avlt* t)
{
    assert(t);
    avlt_recursive_dtor(t->root);
    memset(t, 0, sizeof(*t));
    free(t);
}

static int avlt_update_balance_factors(struct avln* n)
{
    if (!n) return 0;
    int left_height = avlt_update_balance_factors(n->left);
    int rght_height = avlt_update_balance_factors(n->rght);
    n->balance_factor = rght_height - left_height;
    if (left_height > rght_height)
    {
        return left_height + 1;
    } else
    {
        return rght_height + 1;
    }
}

static struct avln* avlt_get_root(struct avln* n)
{
    if (!n) return NULL;
    while (n->uppr)
    {
        n = n->uppr;
    }
    return n;
}

static void avlt_balance_recursive(struct avln* n)
{
    if (!n) return;
    avlt_balance(n);
    avlt_update_balance_factors(avlt_get_root(n));
    avlt_balance_recursive(n->uppr);
}


static struct avln* avlt_recursive_find(struct avln* n, int k, struct avln* p)
{
    if (!n)
    {
        return p;
    } else
    if (n->key == k)
    {
        return n;
    } else
    if (n->key < k)
    {
        return avlt_recursive_find(n->rght, k, n);
    } else
    {
        return avlt_recursive_find(n->left, k, n);
    }
}

int avlt_insert(struct avlt* t, int k)
{
    assert(t);
    struct avln* new_node = NULL;
    if (t->root)
    {
        struct avln* find_res = avlt_recursive_find(t->root, k, NULL);
        assert(find_res);
        if (find_res->key == k)
        {
            return EEXIST;
        }
        new_node = avln_ctor(k, NULL, NULL, find_res, find_res->key > k);
    } else
    {
        new_node = avln_ctor(k, NULL, NULL, NULL, AVLN_DFLT);
    }
    if (new_node == NULL)
    {
        return ENOMEM;
    }
    avlt_balance_recursive(new_node);
    t->root = avlt_get_root(new_node);
    return 0;
}

static struct avln* avlt_find_closest(struct avln* n, int key, struct avln* c)
{
    if (!n) return c;
    assert(c);
    int current_diff = abs(n->key - key);
    int found_diff   = abs(c->key - key);
    struct avln* closest = NULL;
    int closest_diff = 0;
    if (current_diff < found_diff)
    {
        closest = n;
        closest_diff = current_diff;
    } else
    {
        closest = c;
        closest_diff = found_diff;
    }
    struct avln* found_left = avlt_find_closest(n->left, key, closest);
    struct avln* found_rght = avlt_find_closest(n->rght, key, closest);
    int diff_left = abs(found_left->key - key);
    int diff_rght = abs(found_rght->key - key);
    if (diff_left < closest_diff)
    {
        return found_left;
    } else
    if (diff_rght < closest_diff)
    {
        return found_rght;
    } else
    {
        return closest;
    }
}

static struct avln* avlt_delete_recursive(struct avln* n)
{
    struct avln* parent = NULL;
    if (!n->left && !n->rght)
    {
        parent = n->uppr;
        avln_dtor(n);
        avlt_balance_recursive(parent);
        return parent;
    } else
    {
        int left_height = avlt_update_balance_factors(n->left);
        int rght_height = avlt_update_balance_factors(n->rght);
        struct avln* closest_key_node = NULL;
        if (left_height > rght_height)
        {
            closest_key_node = avlt_find_closest(n->left, n->key, n->left);
        } else
        {
            closest_key_node = avlt_find_closest(n->rght, n->key, n->rght);
        }
        n->key = closest_key_node->key;
        parent = closest_key_node->uppr;
        avlt_delete_recursive(closest_key_node);
        avlt_balance_recursive(parent);
        return n;
    }
}

int avlt_delete(struct avlt* t, int k)
{
    assert(t);
    struct avln* find_res = avlt_recursive_find(t->root, k, NULL);
    if ((!find_res) || (find_res->key != k))
    {
        return ENOKEY;
    }
    struct avln* existing_node = avlt_delete_recursive(find_res);
    t->root = avlt_get_root(existing_node);
    return 0;
}

struct avln* avlt_find(struct avlt* t, int k)
{
    assert(t);
    assert(t->root);
    struct avln* key_node = avlt_recursive_find(t->root, k, NULL);
    if ((!key_node) || (key_node->key != k))
    {
        return NULL;
    } else
    {
        return key_node;
    }
}

void avlt_for_each_recursive(struct avlt* t,
                             struct avln* n,
                                     int  m,
                                    void* ctx,
                                    void (*do_ptr)(struct avlt* t,
                                                   struct avln* n,
                                                          void* ctx))
{
    assert(t);
    assert(m == MODE_PREFIX ||
           m == MODE_INFIX  ||
           m == MODE_POSTFIX);
    assert(do_ptr);
    if (!n) return;
    switch (m)
    {
        case MODE_PREFIX:
            do_ptr(t, n, ctx);
            avlt_for_each_recursive(t, n->left, m, ctx, do_ptr);
            avlt_for_each_recursive(t, n->rght, m, ctx, do_ptr);
            break;
        case MODE_INFIX:
            avlt_for_each_recursive(t, n->left, m, ctx, do_ptr);
            do_ptr(t, n, ctx);
            avlt_for_each_recursive(t, n->rght, m, ctx, do_ptr);
            break;
        case MODE_POSTFIX:
            avlt_for_each_recursive(t, n->left, m, ctx, do_ptr);
            avlt_for_each_recursive(t, n->rght, m, ctx, do_ptr);
            do_ptr(t, n, ctx);
            break;
    }
}

void avlt_for_each(struct avlt* t,
                           int  m,
                          void* ctx,
                          void (*do_ptr)(struct avlt* t,
                                         struct avln* n,
                                                void* ctx))
{
    assert(t);
    assert(m == MODE_PREFIX ||
           m == MODE_INFIX  ||
           m == MODE_POSTFIX);
    assert(do_ptr);
    avlt_for_each_recursive(t, t->root, m, ctx, do_ptr);
}

int avlt_dump(struct avlt* t)
{
    assert(t);
    if (!t->root)
        return ECHILD;

    static int dump_number = 0;
    dump_number++;

    char dump_file_name[sizeof("./treeDump000.gv") + 1] = {0};
    sprintf(dump_file_name, "./treeDump%03i.gv", dump_number);

    FILE* dump_file = NULL;
    dump_file = fopen(dump_file_name, "w");
    if (!dump_file)
    {
        return EBADF;
    }

    fprintf(dump_file, "digraph G {\n"
                      "graph [ dpi = 300 ]\n");

    avlt_for_each(t, MODE_PREFIX, dump_file, avln_dump);

    fprintf(dump_file, "}\n");

    if (fclose(dump_file) == EOF)
    {
        return EBADF;
    }
#ifdef INTERACTIVE
    char temp[64] = "";
    sprintf(temp, "xdot %s", dump_file_name);
    if (system(temp) != 0)
    {
        return EACCES;
    }
#endif
    return 0;
}
