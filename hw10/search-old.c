#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "search.h"

#define INITIAL_SIZE (1024)

typedef struct position pos;
typedef long long lint;
typedef struct node_t { pos p; int d; int f; } node;
typedef struct heap_t { node** data; lint size; lint numels;} heap;

int h(pos p1, pos p2) { int xd = abs(p1.x - p2.x);
  int yd = abs(p1.y - p2.y); return (xd + yd); }

void swap(node* n1,node* n2) { node t = *n2; *n2 = *n1; *n1 = t; }

int hEmpty(heap *h) {
  return (h->numels == 0);
}

int hNumels(heap *h) {return h->numels;}

void printHeap(heap *h) {
  int i;
  node* temp;
  for (i=0;i<h->numels;i++) {
    temp = h->data[i];
    fprintf(stderr,"%4d %4d %8d %8d\n",temp->f, temp->d, temp->p.x,temp->p.y);
  }
}

node* nodeCreate(pos p, pos t, int dist) {
  node* ptr = malloc(sizeof(node));
  ptr->p = p;
  ptr->d = dist;
  ptr->f = dist + h(p,t);
  return ptr;
}

void heapifyUp(heap* h,int i) {
  int par;
  while(1) {
    par = (i-1)/2;
    if (h->data[i]->f < h->data[par]->f) {
      swap(h->data[i],h->data[par]);
      i = par;
    }
    else break;
  }
}

void heapPush(heap* h, node* ptr) {
  if(h->numels + 5 >= h->size) {
    h->size*=2;
    h->data = realloc(h->data,h->size);
  }
  h->data[h->numels] = ptr;
  h->numels+=1;
  heapifyUp(h,h->numels-1);
}

void heapifyDown(heap* h,int i) {
  int l,r,small;
  int len=h->numels;
  node** data = h->data;
  while(1) {
    l = 2*i+1;
    r = 2*i+2;
    small = i;
    if (l < len && data[l]->f < data[small]->f) small = l;
    if (r < len && data[r]->f < data[small]->f) small = r;
    if (small != i) {
      swap(data[i],data[small]);
      i = small;
    }
    else break;
  }
}

node* heapPop(heap *h) {
  if (hEmpty(h)) return NULL;
  node* ptr = h->data[0];
  h->numels-=1;
  h->data[0] = h->data[h->numels];
  heapifyDown(h,0);
  return ptr;
}

heap* heapCreate(size_t s) {
  heap *h = malloc(sizeof(heap));
  h->data = malloc(s*sizeof(node*));
  h->numels = 0;
  h->size = s;
  return h;
}

void heapDestroy(heap *h) {
  if (h) {
    if (h->data) {
      int i;
      for (i=0;i<h->numels;i++)
        if(h->data[i]) free(h->data[i]);
      free(h->data);
    }
    free(h);
  }
}


int posEq(pos p1, pos p2) {
  return (p1.x == p2.x && p1.y == p2.y);
}

int search (pos source, pos target, int (*blocked)(pos)) {
  heap *h = heapCreate(1);//INITIAL_SIZE);
  node *temp = nodeCreate(source, target, 0);
  //create node
  heapPush(h,temp);

/*
  while (!hEmpty(h)) {
    temp = heapPop();
    if (posEq(temp.p, target)) return temp.d;
    

  }
*/

  heapDestroy(h);
  return 0;
}


