#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "search.h"
#include "mystuff.h"

int main() {
  const int SIZE = 30;
  pos poslist[SIZE];
  pos dest;
  dest.x = 10;
  dest.y = 10;
  int i;
  heap *h = heapCreate(1);
  srand(0);

  for (i=0;i<SIZE;i++) {
    poslist[i].x = rand();
    poslist[i].y = rand();
  }

  node* temp;
  for (i=0;i<SIZE;i++) {
    temp = nodeCreate(poslist[i],dest,0);
    heapPush(h,temp);
  }
  assert( hNumels(h) == SIZE);

  printHeap(h);

  heapDestroy(h);
  return 0;
}

