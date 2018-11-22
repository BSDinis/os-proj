/*	
 *	
 */	
 #include <stdio.h>	
#include <stdlib.h>	
#include "lib/hashtable.h"	
 typedef struct {	
  int a;	
  int b;	
} pair;  	
 ssize_t hash1(const ssize_t cap, const void * arg) {	
  pair * p = (pair *) arg;	
  return p->a % cap;	
}	
 ssize_t hash2(const ssize_t cap, const void * arg) {	
  if (arg == NULL) return 3;  	
   pair * p = (pair *) arg;	
  return p->a % 4 + 2;	
}	
 int equals(const void *_1, const void *_2)	
{	
  if (_1 == NULL || _2 == NULL) return 1;	
  pair * c = (pair *) _1;	
  pair * d = (pair *) _2;	
  return (c->a == d->a) ? 1 : 0;	
}	
 int main()	
{	
  hashtable_t t = hashtable_(20, hash1, hash2, equals);	
  for (int i = 0; i < 37; i++) {	
    pair * p = malloc(sizeof(pair));	
    p->a = i * 3;	
    p->b = i * 2;	
    hashtable_add(t, (void *) p);	
  }	
   for (int i = 0; i < 37; i++) {	
    pair * incomplete = malloc(sizeof(pair));	
    incomplete->a = i * 3;	
    incomplete->b = 0;	
    pair * complete = hashtable_lookup(t, (void *)incomplete);	
    printf("(%d, %d) -> (%d, %d)\n", incomplete->a, incomplete->b, complete->a, complete->b);	
     if (i % 2) {	
      pair * p = (pair *) hashtable_rem(t, (void *) incomplete);	
      if (hashtable_lookup(t, (void *) incomplete) != NULL) {	
        fprintf(stderr, "DID NOT REM %d\n", i);	
      }	
      else {	
        printf("REMD (%d, %d)\n", p->a, p->b);	
        free(p);	
      }	
    }	
     free(incomplete);	
  }	
  	
  free_hashtable(t);	
  return 0;	
}	

