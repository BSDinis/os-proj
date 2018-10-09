/*
 * test simple list
 */

#include "lib/simple_list.h"
#include <stdio.h>
#include <stdlib.h>

int op(void * arg) 
{
  int *a = (int *) arg;
  printf(" - %d\n", *a);
  free(a);
  return 0;
}

int main()
{
  simple_list_t l = simple_list_();

  for (int i = 0; i < 1000; i++) {
    int *p = malloc(sizeof(int));
    printf("adding %d\n", i);
    *p = i;

    simple_list_pushback(l, (void *) p);
  }
  

  simple_list_transverse(l, op);
  free_simple_list(l);
  return 0;
}


