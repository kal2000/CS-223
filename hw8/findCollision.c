#include "findCollision.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <limits.h>

typedef struct dict *Dict;
typedef unsigned int uint;

typedef struct mytype{
  uint data;
  char found;
} flagdata;

/* create a new empty dictionary */
Dict DictCreate(int x);

/* destroy a dictionary */
void DictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void DictInsert(Dict, uint key, uint value);

/* return the most recently inserted value associated with a key */
/* or -1 if no matching key is present */
flagdata *DictSearch(Dict, uint key);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void DictDelete(Dict, uint key);


struct elt {
    struct elt *next;
    uint key;
    uint value;
};

struct dict {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt **table;
};

/* dictionary initialization code used in DictCreate */
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

    return d;
}

Dict
DictCreate(int x)
{
    return internalDictCreate(x);
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

/* insert a new key-value pair into an existing dictionary */
void
DictInsert(Dict d, uint key, uint value)
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
}

/* return the most recently inserted value associated with a key */
/* or -1 if no matching key is present */
flagdata *
DictSearch(Dict d, uint key)
{
    struct elt *e;
    flagdata *p = malloc(sizeof(flagdata));
    p->data = 0;
    p->found = 0;

    for(e = d->table[key % d->size]; e != 0; e = e->next) {
        if(e->key==key) {
            /* got it */
            p->found = 1;
            p->data = e->value;
            break;
        }
    }
  
    return p;
}

void findCollision(unsigned int (*f)(unsigned int),unsigned int *x1, unsigned int *x2) {

  srand(time(0));
  Dict d,tried;
  const unsigned int MASK = 0x7fffffff;
  const int EXPECTED_RUNS = 100000; //more than 65536
  d = DictCreate(EXPECTED_RUNS);
  //hash table, keys are function's return values and values are the input values

  tried = DictCreate(EXPECTED_RUNS);
  //hash table, keys are input values that have been tried
  
  flagdata *ptr = NULL;
  unsigned int res;
//  fprintf(stderr,"%u %u %d\n",RAND_MAX,UINT_MAX,INT_MAX);

  while(1) {
    do {
      *x1 = rand()*2+rand()%2;
      if(ptr!=NULL) { free(ptr); ptr=NULL;}
      ptr = DictSearch(tried,*x1);
    } while(ptr->found);

//    if (*x1>INT_MAX) fprintf(stderr,"%u\n",*x1);
    res = f(*x1)&MASK;
    DictInsert(tried,*x1,res);

    if(ptr!=NULL) { free(ptr); ptr=NULL;}
    ptr = DictSearch(d,res);
    if (ptr->found) {
      *x2 = ptr->data;
      free(ptr); ptr=NULL;
      DictDestroy(d);
      DictDestroy(tried);
      return;
    }
    else {
      DictInsert(d,res,*x1);
      free(ptr); ptr=NULL;
    }
  }


}

