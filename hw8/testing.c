#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "myhash.c"

int main() {
  Dict d,a;
  unsigned int i;

  d = DictCreate(100000);

  srand(27);
  for (i=0;i<1000000;i++)
    DictInsert(d,i,i+1);
  
  flagdata* ptr = DictSearch(d,1000001);

  printf("%d\n",ptr->found);
  printf("Done\n");

  return 0;
}
