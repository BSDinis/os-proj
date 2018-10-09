/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * simple_list.c
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

#include "simple_list.h"
#include <stdlib.h>

struct node {
  void *item;
  struct node * next;
};

typedef struct node * node_t;

struct simple_list {
  node_t head;
  node_t tail;
};


// create simple list
struct simple_list * simple_list_()
{
  struct simple_list * l = malloc(sizeof(struct simple_list));
  if (l == NULL) {
    fprintf(stderr, "simple_list: malloc failed\n");
    return NULL; 
  }
  
  l->head = malloc(sizeof(struct node));
  if (l->head == NULL) {
    fprintf(stderr, "simple_list: malloc failed\n");
    free(l);
    return NULL; 
  }

  l->tail = malloc(sizeof(struct node));
  if (l->tail == NULL) {
    fprintf(stderr, "simple_list: malloc failed\n");
    free(l->head);
    free(l);
    return NULL; 
  }

  l->head->item = NULL;
  l->tail->item = NULL;
  l->head->next = l->tail;
  l->tail->next = NULL;

  return l;
}

// destroy simple list
//
// items must be freed by now (use transverse)
void free_simple_list(struct simple_list * l) 
{
  if (l == NULL) return;

  node_t current;
  node_t next = l->head;
  while (next != NULL) {
    current = next;
    next = current->next;
    free(current);
  }

  free(l);
}


// add to the list
int simple_list_pushback(struct simple_list * l, void * item)
{
  if (l == NULL || item == NULL) return -1;

  node_t new = malloc(sizeof(struct node));
  if (new == NULL) {
    fprintf(stderr, "simple_list_pushback: malloc failed\n");
    return -1;
  }
  
  new->item = item;
  new->next = l->head->next;
  l->head->next = new;
  

  return 0;
}


// transverse: perform the same function on all the items
int simple_list_transverse(struct simple_list * l, int (*operation)(void *))
{
  if (l == NULL || operation == NULL) return -1;

  node_t node = l->head->next;

  while (node != l->tail) {
    if (operation(node->item) == -1) {
      return -1;
    }

    node = node->next;
  }
  
  return 0;
}

#endif // __SIMPLE_LIST_H
