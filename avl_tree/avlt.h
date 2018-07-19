#ifndef AVLT_H_INCLUDED
#define AVLT_H_INCLUDED
#include "common.h"
#include "avln.h"

/**
  * AVL self-balancing Tree
  */
struct avlt
{
    /**
      * AVLT root node
      */
    struct avln* root;
};

/**
  * AVLT constructor
  * @param k Key to insert initially
  * @return \n
  * Not NULL - pointer to constructed avlt struct
  * NULL - not enough memory
  * @see struct avlt
  */
struct avlt* avlt_ctor(int k);

/**
  * AVLT destructor
  * @param t Tree to destruct
  * @see struct avlt
  */
void avlt_dtor(struct avlt* t);

/**
  * Insert key in AVLT
  * @param t Tree to insert key in
  * @param k Key to insert
  * @return \n
  * Zero on success
  * EEXIST - key exists
  * ENOMEM - not enough memory
  * @see struct avlt
  */
int avlt_insert(struct avlt* t, int k);

/**
  * Delete key from AVLT
  * @param t Tree to delete key from
  * @param n Key to delete
  * @return \n
  * Zero on success
  * ENOKEY - key not found
  * @see struct avlt
  */
int avlt_delete(struct avlt* t, int k);

/**
  * Find key in AVLT
  * @param t Tree to find key in
  * @param n Key to find
  * @return \n
  * Not NULL - pointer to found node
  * NULL - key not found
  * @see struct avlt
  */
struct avln* avlt_find(struct avlt* t, int k);

/**
  * Perform do operation for each tree node
  * @param t Tree to perform operation on
  * @param m Tree visiting mode
  * @param ctx Context to pass to 'do' function
  * @param do_ptr Function to execute for each node
  * @see avlt_visit_mode
  * @see struct avlt
  */
void avlt_for_each(struct avlt* t,
                           int  m,
                          void* ctx,
                          void (*do_ptr)(struct avlt*,
                                         struct avln*,
                                                void*));

/**
  * Dumps AVL tree content to the dot-formatted file
  * @param t Tree to dump
  * @return \n
  * Zero on success
  * ECHILD - no root node
  * EBADF - if fopen of fclose failed
  * EACCES - if system failed
  * @see struct avlt
  */
int avlt_dump(struct avlt* t);

#endif
