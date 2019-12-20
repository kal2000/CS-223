#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "search.h"
#include "dict.h"
#include "pq.h"

typedef long long lint;
typedef struct nodet { struct position p; int d; int f; } node;
typedef struct heapt { node** data; lint size; lint numels;} heap;

int h(struct position p1, struct position p2) {
  int xd = abs(p1.x - p2.x);
  int yd = abs(p1.y - p2.y);
  return (xd + yd);
}

void swap(node* n1,node* n2) { node t = *n2; *n2 = *n1; *n1 = t; }


int posEq(struct position p1, struct position p2) {
  return (p1.x == p2.x && p1.y == p2.y);
}

int fcomp(const void *n1,const void *n2) {
  return ((const node*) n1)->f - ((const node*) n2)->f;
}

int pos2key(struct position p) {
  int k = abs(p.x*97 + p.y*53);
  return k;
}

void push(PQ p, struct position s, struct position t, int d) {
  node n;
  n.p = s;
  n.d = d;
  n.f = d+h(s,t);
//  fprintf(stderr,"s.x = %d | s.y = %d | d = %d | f = %d\n",s.x,s.y,d,n.f);
  pq_insert(p,&n);
}

int neighb(int curx, int cury, int i, int j) {
  if ( (curx == i && ( (cury !=INT_MAX && cury+1==j) ||
                       (cury !=INT_MIN && cury-1==j) ) ) ||
       (cury == j && ( (curx !=INT_MAX && curx+1==i) ||
                       (curx !=INT_MIN && curx-1==i) ) ) ) {
    return 1;
  }
  else return 0;
}

int search (struct position source, struct position target, int (*blocked)(struct position)) {
  int i,j,curx,cury,ans,curd;
  struct position temppos;
  node curnode;
  PQ open = pq_create(sizeof(node), fcomp);
  push(open,source,target,0);

  Dict closed;
  closed = DictCreate();

  while(!pq_is_empty(open)) {
    pq_delete_min(open, &curnode);
//    fprintf(stderr,"At %d %d d = %d || f = %d\n",curnode.p.x,curnode.p.y,curnode.d,curnode.f);
    if ( DictSearch(closed, pos2key(curnode.p), curnode.p ) ) {
      continue;
    }
    if (posEq(curnode.p, target)) {
      ans = curnode.d;
      DictDestroy(closed);
      pq_destroy(open);
      return ans;
    }

    DictInsert(closed, pos2key(curnode.p), curnode.p);
    curx = curnode.p.x;
    cury = curnode.p.y;
    curd = curnode.d;
//    fprintf(stderr,"Processing\n");
    for (i=curx-1;i<=curx+1;i++) {
      for (j=cury-1;j<=cury+1;j++) {
        if (neighb(curx,cury,i,j)==0) continue;
        temppos.x = i;
        temppos.y = j;
//        fprintf(stderr,"x = %d | y = %d | d = %d\n",i,j,curd+1);
        if ( blocked(temppos) ) {
//          fprintf(stderr,"blocked at %d %d\n",temppos.x,temppos.y);
          continue;
        }
        if ( DictSearch(closed,pos2key(temppos), temppos) ) {
          continue;
        }
        else {
          push(open,temppos,target,curd+1);
        }
      }
    }
  }

  DictDestroy(closed);
  pq_destroy(open);
  return NO_PATH;
}


