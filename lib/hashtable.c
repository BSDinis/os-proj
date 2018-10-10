/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * hashtable.c
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 * Simple hashtable (with double hashing, resizing)
 * Implements:
 *   -insert 
 *   -remove 
 *   -lookup
 *
 * return codes: all ints indicate status (0 -> success ; -1 -> error)
 */

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SENTINEL ((void *)0x1) // invalid pointer

struct hashtable {
  ssize_t capacity;
  ssize_t size;
  ssize_t (*hash1)(const ssize_t, const void*);
  ssize_t (*hash2)(const ssize_t, const void*);
  int (*equals)(const void *, const void*); // everything is equal to NULL
  void ** table;
};

// create hashtable
struct hashtable * hashtable_(
    const ssize_t initial_size,
    ssize_t (*hash1)(const ssize_t, const void *),
    ssize_t (*hash2)(const ssize_t, const void *),
    int (*equals)(const void *, const void *)
    )
{
  if (initial_size <= 0 
      || hash1 == NULL 
      || hash2 == NULL 
      || equals == NULL ) return NULL;
  struct hashtable * t = malloc(sizeof(struct hashtable));

  if (t == NULL) {
    fprintf(stderr, "hashtable_: malloc failed\n");
    return NULL;
  }
  
  t->capacity = initial_size;
  t->size = 0;
  t->hash1 = hash1;
  t->hash2 = hash2;
  t->equals = equals;

  t->table = malloc(t->capacity * sizeof(void *));
  if (t->table == NULL) {
    fprintf(stderr, "hashtable_: malloc failed\n");
    free(t);
    return NULL;
  }
  
  memset(t->table, 0, t->capacity * sizeof(void *));

  return t;
}


// lookup by index
static ssize_t hashtable_index_lookup(const struct hashtable *t, const void * item)
{
  if (t == NULL || item == NULL) 
    return -1;

  ssize_t index = t->hash1(t->capacity, item);
  const ssize_t step = t->hash2(t->capacity, item);

  while (t->table[index] == SENTINEL || 
      (t->table[index] != NULL
       && !t->equals(t->table[index], item))) {
    index += step;
    index %= t->capacity;
  }

  return index;
}

// add to table 
static int hashtable_add_table(void ** table, const ssize_t capacity, ssize_t index, const ssize_t step, void * item)
{
  if (table == NULL ||
      capacity <= 0 ||
      index < 0 ||
      index >= capacity ||
      item == NULL)
    return -1;

  while (table[index] != NULL && table[index] != SENTINEL) {
    index += step;
    index %= capacity;
  }
  table[index] = item;

  return 0;
}


// resize
static int hashtable_resize(struct hashtable *t)
{
  if (t == NULL) return -1;

  ssize_t new_capacity = t->capacity * 2;
  void **new_table = malloc(new_capacity * sizeof(void *));

  if (new_table == NULL) {
    fprintf(stderr, "hashtable_resize: malloc failed\n");
    return -1;
  }

  memset(new_table, 0, new_capacity * sizeof(void *));
  
  for (ssize_t i = 0; i < t->capacity; i++) {
    if (t->table[i] != NULL && t->table[i] != SENTINEL) {
      void * item = t->table[i];
      ssize_t index = t->hash1(new_capacity, item);
      ssize_t step = t->hash2(new_capacity, item);

      if (hashtable_add_table(new_table, new_capacity, index, step, item) == -1) {
        free(new_table);
        return -1;
      }
    }
  }

  free(t->table);
  t->table = new_table;
  t->capacity = new_capacity;
  return 0;
}



// destroy hashtable
// frees pointers
void free_hashtable(struct hashtable *t)
{
  if (t == NULL) return;
  for (ssize_t i = 0; i < t->capacity; i++)
    if (t->table[i] != NULL && t->table[i] != SENTINEL)
      free(t->table[i]);

  free(t->table);
  free(t);
}


// add to the hashtable
int hashtable_add(struct hashtable *t, void * item)
{
  if (t == NULL || item == NULL)
    return -1;

  if (t->size * 2 > t->capacity) {
    if (hashtable_resize(t) == -1) {
      return -1;
    }
  }
  
  ssize_t index = t->hash1(t->capacity, item);
  ssize_t step = t->hash2(t->capacity, item);

  if (hashtable_add_table(t->table, t->capacity, index, step, item) == -1) {
    return -1;
  }

  t->size++;
  return 0;
}


// remove from the hashtable (returns item - NULL if not found)
void * hashtable_rem(struct hashtable * t, void * item)
{
  if (t == NULL || item == NULL)
    return NULL;

  ssize_t index = hashtable_index_lookup(t, item);
  if (index < 0) 
    return NULL;

  void * rem_item = t->table[index];
  if (rem_item != NULL) {
    t->table[index] = SENTINEL;
    t->size--;
  }

  return rem_item;
}


// lookup: returns void *, the item found (or NULL if not found) 
// allows for a complex structure with n elements to be indexed by only one of them
//
// so, the filled struct can be indexed by one of the elements and, on lookup,
// the function can be given an incomplete struct (with the same index)
//
// This depends on the hash functions given
void * hashtable_lookup(const struct hashtable *t, const void * item) 
{
  if (t == NULL || item == NULL)
    return NULL;

  ssize_t index = hashtable_index_lookup(t, item);
  if (index == -1) 
    return NULL;

  return t->table[index];
}


ssize_t hashtable_capacity(const struct hashtable *t) {
  if (t == NULL) return -1;
  return t->capacity;
}

ssize_t hashtable_size(const struct hashtable *t) {
  if (t == NULL) return -1;
  return t->size;
}

#endif // __HASHTABLE_H
