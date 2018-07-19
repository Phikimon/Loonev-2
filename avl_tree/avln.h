#ifndef AVLN_H_INCLUDED
#define AVLN_H_INCLUDED
#include "common.h"
#include "avlt.h"

struct avlt;
/**
  * AVL self-balancing tree Node
  */
struct avln
{
    int key; /*< Key to store */

    /**
      * Difference between heights
      * of left and right subtrees
      */
    int balance_factor;

    struct avln* uppr; /*< Parent node pointer */
    struct avln* left; /*< Left node pointer */
    struct avln* rght; /*< Right node pointer */
};

/**
  * AVLN constructor
  * @param k Key to insert initially
  * @param uppr Parent node
  * @param left Left daughter node
  * @param rght Right daughter node
  * @return \n
  * Not NULL - pointer to constructed avln struct
  * NULL - not enough memory
  * @see struct avln
  */
struct avln* avln_ctor(int k,
                       struct avln* left,
                       struct avln* rght,
                       struct avln* uppr,
                       int          to_left);

/**
  * AVLN constructor
  * @param n Node to destruct
  * @see struct avln
  */
void avln_dtor(struct avln* n);

/**
  * Is this node a left child?
  * @param n Node pointer
  * @return \n
  * +1 if is left
  *  0 if is rght
  * -1 if there is no parent
  * @see struct avln
  */
int avln_is_left(struct avln* n);

/**
  * Redipatches node's and both parents nodes' \n
  * pointers. Also puts NULL in old parent's \n
  * left/rght node pointer.
  * @param n Node to change parent of
  * @param op Old parent
  * @param n_is_left Is n left child?
  * @param np New parent
  * @param to_left Make n left child
  * @return Was the n node left?
  * @see struct avln
  */
int avln_new_parent(struct avln* n,
                   struct avln** opp,
                    struct avln* np,
                    int          to_left);

/**
  * Prints node content on screen
  * @param t Tree, which node belongs to. \n
  * Not used, added for compatibility with
  * avlt_for_each function
  * @param n Node to print content of
  * @param stream Stream to print content to in \n
  * stdio.h format. Is casted to FILE*.
  * @see struct avln
  * @see avlt_for_each
  */
void avln_dump(struct avlt* t,
               struct avln* n,
               void* stream);

#endif
