#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "avln.h"

struct avln* avln_ctor(int k,
                       struct avln* left,
                       struct avln* rght,
                       struct avln* uppr,
                       int          to_left)
{
    struct avln* new_avln = (struct avln*)calloc(1, sizeof(*new_avln));
    if (new_avln == NULL)
    {
        return NULL;
    }
    new_avln->key  = k;
    new_avln->left = left;
    new_avln->rght = rght;
    new_avln->uppr = uppr;

    if (!uppr)
    {
        return new_avln;
    }

    if (to_left == AVLN_LEFT)
    {
        uppr->left = new_avln;
    } else
    if (to_left == AVLN_RGHT)
    {
        uppr->rght = new_avln;
    }

    return new_avln;
}

void avln_dtor(struct avln* n)
{
    assert(n);
    if (n->left)
    {
        n->left->uppr = NULL;
    }
    if (n->rght)
    {
        n->rght->uppr = NULL;
    }

    int is_left = avln_is_left(n);
    if (is_left == AVLN_LEFT)
    {
        n->uppr->left = NULL;
    } else
    if (is_left == AVLN_RGHT)
    {
        n->uppr->rght = NULL;
    }

    n->key = n->balance_factor = 0;
    n->left = n->rght = n->uppr = NULL;
    free(n);
}

int avln_is_left(struct avln* n)
{
    assert(n);
    if (!(n->uppr))
    {
        return AVLN_DFLT;
    } else
    if (n->uppr->left == n)
    {
        return AVLN_LEFT;
    } else
    if (n->uppr->rght == n)
    {
        return AVLN_RGHT;
    } else
    {
        return AVLN_DFLT;
    }
}

int avln_new_parent(struct avln* n,
                   struct avln** opp,
                    struct avln* np,
                             int to_left)
{
    int n_is_left = AVLN_DFLT;
    if (n)
    {
        if (!opp)
        {
            n_is_left = avln_is_left(n);
            if (n_is_left == AVLN_LEFT)
            {
                n->uppr->left = NULL;
            } else
            if (n_is_left == AVLN_RGHT)
            {
                n->uppr->rght = NULL;
            }
        }
        n->uppr = np;
    }
    if (opp)
    {
        *opp = NULL;
    }

    if (np)
    {
        if (to_left == AVLN_LEFT)
        {
            np->left = n;
        } else
        if (to_left == AVLN_RGHT)
        {
            np->rght = n;
        }
    }

    return n_is_left;
}

void avln_dump(struct avlt* t,
               struct avln* n,
               void* vstream)
{
    assert(n);
    assert(vstream);
    FILE* stream = vstream;

    fprintf(stream, "treeNode_%p [label=\"", (void*)n);

    fprintf(stream, "Node [%p]\\l", (void*)n);
    fprintf(stream, "{\\l");
    fprintf(stream, "\tuppr = %p\\l", (void*)n->uppr);
    fprintf(stream, "\tleft = %p\\l", (void*)n->left);
    fprintf(stream, "\trght = %p\\l", (void*)n->rght);
    fprintf(stream, "\tkey  = %d\\l", n->key);
    fprintf(stream, "\tbf   = %d\\l", n->balance_factor);
    fprintf(stream, "}\\l");

    fprintf(stream, "\"]\n");

    if (!n->left)
    {
        fprintf(stream, "treeNode_%p_left [label=\"(nil)\"]\n", (void*)n);
        fprintf(stream, "treeNode_%p -> treeNode_%p_left\n", (void*)n, (void*)n);
    }
    else
    {
        fprintf(stream, "treeNode_%p -> treeNode_%p\n", (void*)n, (void*)n->left);
    }

    if (!n->rght)
    {
        fprintf(stream, "treeNode_%p_rght [label=\"(nil)\"]\n", (void*)n);
        fprintf(stream, "treeNode_%p -> treeNode_%p_rght\n", (void*)n, (void*)n);
    }
    else
    {
        fprintf(stream, "treeNode_%p -> treeNode_%p\n", (void*)n, (void*)n->rght);
    }

    if (n->uppr)
    {
        fprintf(stream, "treeNode_%p -> treeNode_%p\n", (void*)n, (void*)n->uppr);
    }
}
