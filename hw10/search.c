#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "search.h"
#include <assert.h>

typedef long long lint;
typedef struct nodet { struct position p; int d; int f; } node;
typedef struct heapt { node** data; lint size; lint numels;} heap;
typedef struct dict *Dict;

/* generic priority queue */

typedef struct pq *PQ;

/* create a new empty priority queue */
/* element_length is the size of an element in bytes */
/* compare is a comparision function that returns
 * <0 if its first argument is less than its second
 *  0 if they are equal
 * >0 if the first argument is greater than the second. */ 
PQ pq_create(int element_length, int (*compare)(const void *, const void *));

/* free a priority queue */
void pq_destroy(PQ);

/* add an element to the priority queue */
/* the contents of the element are COPIED from elt */
void pq_insert(PQ, const void *elt);

/* returns nonzero if PQ is empty */
int pq_is_empty(PQ);

/* delete the minimum element of the priority queue */
/* and COPY its contents to retval */
/* it is an error to call this on an empty queue */
void pq_delete_min(PQ, void *retval);

/* utility function: blows up if heap invariant is violated */
void pq_sanity_check(PQ);

/* create a new empty dictionary */
Dict DictCreate(void);

/* destroy a dictionary */
void DictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void DictInsert(Dict, int key, struct position value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
int DictSearch(Dict, int key, struct position value);


struct elt {
    struct elt *next;
    int key;
    struct position value;
};

struct dict {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt **table;
};

#define INITIAL_SIZE (8192)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

/* dictionary initialization code used in both DictCreate and grow */
Dict
internalDictCreate(int size)
{
    Dict d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != 0);

    for(i = 0; i < d->size; i++) d->table[i] = 0;
    
    
//    fprintf(stderr,"size = %d\n",d->size);

    return d;
}

Dict
DictCreate(void)
{
    return internalDictCreate(INITIAL_SIZE);
}

void
DictDestroy(Dict d)
{
    int i;
    struct elt *e;
    struct elt *next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static void
grow(Dict d)
{
    Dict d2;            /* new dictionary we'll create */
    struct dict swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = internalDictCreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            DictInsert(d2, e->key, e->value);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call DictDestroy on d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;

    DictDestroy(d2);
}

/* insert a new key-value pair into an existing dictionary */
void
DictInsert(Dict d, int key, struct position value)
{
    struct elt *e;
    unsigned long h;

    e = malloc(sizeof(*e));
    assert(e);

    e->key = key;
    e->value = value;
    h = key % d->size;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
int posEq2(struct position p1, struct position p2) {
  return (p1.x == p2.x && p1.y == p2.y);
}

int
DictSearch(Dict d, int key, struct position value)
{
    struct elt *e;
//    fprintf(stderr,"key = %d size = %d\n",key,d->size);
    for(e = d->table[key%d->size]; e != 0; e = e->next) {
        if(posEq2(e->value,value)) {
            /* got it */
            return 1;
        }
    }

    return 0;
}



struct pq {
    int element_length;
    int (*compare)(const void *, const void *);
    int n;      /* number of elements */
    int size;   /* number of slots in data */
    void *swap_space;   /* element_length bytes used for swapping */
    void *data;
};

#define PQ_INITIAL_SIZE (128)

PQ pq_create(int element_length, int (*compare)(const void *, const void *))
{
    PQ pq;

    pq = malloc(sizeof(*pq));
    assert(pq);

    pq->element_length = element_length;
    pq->compare = compare;
    pq->n = 0;
    pq->size = PQ_INITIAL_SIZE;

    pq->swap_space = malloc(pq->element_length);
    assert(pq->swap_space);

    pq->data = malloc(pq->element_length * pq->size);
    assert(pq->data);

    return pq;
}

void
pq_destroy(PQ pq)
{
    free(pq->data);
    free(pq->swap_space);
    free(pq);
}

int
pq_is_empty(PQ pq)
{
    return pq->n == 0;
}

/* Child(i, 0) and Child(i, 1) are children of i */
/* Parent(i) is parent of i */
#define Child(i, x) (2*(i)+1+(x))
#define Parent(i) (((i)-1)/2)

#define NUM_CHILDREN (2)

/* compute the address of position i in the data field */
#define REF(pq, i) ((void *) (((char *) (pq)->data) + (pq)->element_length * i))

/* swap elements at indexes i1 and i2 */
static void
pq_swap(PQ pq, int i1, int i2)
{
    memcpy(pq->swap_space, REF(pq, i1), pq->element_length);
    memcpy(REF(pq, i1), REF(pq, i2), pq->element_length);
    memcpy(REF(pq, i2), pq->swap_space, pq->element_length);
}

void
pq_insert(PQ pq, const void *elt)
{
    int floater;                /* new element */

    while(pq->n + 1 > pq->size) {
        pq->size *= 2;
        pq->data = realloc(pq->data, pq->element_length * pq->size);
        assert(pq->data);
    }

    /* copy the new element in */
    floater = pq->n++;
    memcpy(REF(pq, floater), elt, pq->element_length);

    /* float it up until it is at the top */
    /* or it is no smaller than its parent */
    while(floater > 0 && pq->compare(REF(pq, floater), REF(pq, Parent(floater))) <= 0) {
        /* it's smaller than its parent */
        pq_swap(pq, floater, Parent(floater));
        floater = Parent(floater);
    }
}

void
pq_delete_min(PQ pq, void *retval)
{
    int floater;        /* previous loser floating down */
    int small_child;    /* smaller child of floater */
    
    assert(!pq_is_empty(pq));

    /* first copy out the winner */
    memcpy(retval, REF(pq, 0), pq->element_length);

    --(pq->n);

    if(pq_is_empty(pq)) {
        /* pq empty, nothing to do */
        return;
    }

    /* else */
    memcpy(REF(pq, 0), REF(pq, pq->n), pq->element_length);

    floater = 0;

    for(;;) {
        /* find smaller child of floater */
        if(Child(floater, 0) >= pq->n) {
            return;     /* no children, bail out */
        } else if(Child(floater, 1) >= pq->n) {
            small_child = Child(floater, 0);
        } else if(pq->compare(REF(pq, Child(floater, 0)), REF(pq, Child(floater, 1))) < 0) {
            small_child = Child(floater, 0);
        } else {
            small_child = Child(floater, 1);
        }

        /* is floater <= small_child? */
        if(pq->compare(REF(pq, floater), REF(pq, small_child)) <= 0) {
            /* yes, we are done */
            return;
        } else {
            /* no, swap and continue floating down */
            pq_swap(pq, floater, small_child);
            floater = small_child;
        }
    }
}



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


