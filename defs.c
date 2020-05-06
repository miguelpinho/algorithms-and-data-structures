#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

int *new_int( int i )
{
  int *new;

  new = (int*) malloc(sizeof(int));
  mem_check(new);
  *new = i;

  return new;
}

int cmp_int( Item a , Item b )
{
  return *((int*) a) - *((int*) b);
}

void print_int( Item a , FILE *fp )
{
  fprintf( fp , "%d-" , *((int*) a) );
}
