/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * simple_list.h
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 * Simple list that only allows head insert
 * Allows transverse (with an input function)
 *
 *
 * return codes: all ints indicate status (0 -> success ; -1 -> error)
 */

#ifndef __SIMPLE_LIST_H
#define __SIMPLE_LIST_H

typedef struct simple_list * simple_list_t;

// create simple list
simple_list_t simple_list_();

// destroy simple list
void free_simple_list(simple_list_t);

// add to the list
int simple_list_pushback(simple_list_t, void * item);

// transverse: perform the same function on all the items
int simple_list_transverse(simple_list_t, int (*operation)(void *));

#endif // __SIMPLE_LIST_H
