#include "superstring.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct superstring {
  size_t len;
  char* data;
  char* upcased;
};

typedef struct node_t {
  char c;
  struct node_t* next;
}node;

void nodeInit(node* n, char c) {
  n->c = c;
  n->next=NULL;
}

typedef struct nodeq_t {
  node* head;
  node* tail;
}nodeq;

void qInit(nodeq *q) {
  q->head = NULL;
  q->tail = NULL;
}

void qAdd(nodeq* q, node* n) {
  if(q->head==NULL) {
    q->head=n;
    q->tail=n;
  }
  else {
    q->tail->next = n;
    q->tail = n;
  }
}

node* qPop(nodeq* q){
  if(q->head==NULL)
    return NULL;
  node* temp = q->head;
  q->head = q->head->next;
  if(q->head == NULL)
    q->tail = NULL;
  temp->next = NULL;
  return temp;
}

void freeQNodes(nodeq* q) {
  node* temp=q->head;
  while(temp!=NULL) {
    q->head=q->head->next;
    free(temp);
    temp=q->head;
  }
  q->tail = NULL;
}

Superstring * superstringCreate(size_t size) {
  if (size == 0)
    return 0;
  Superstring* s = malloc(sizeof(Superstring));
  s->len = size;
  char *ptr = malloc( (size+1)*sizeof(char) );
  s->data = ptr;
  ptr = malloc( (size+1)*sizeof(char));
  s->upcased = ptr;

  int i;
  for(i=0;i<=size;i++) {
    s->data[i] = '\0';
  }

  return s;
}

void superstringDestroy(Superstring *s) {
  if(s != NULL) {
    if(s->data != NULL)
      free(s->data);
    if(s->upcased != NULL)
      free(s->upcased);
    free(s);
  }
}

Superstring * superstringFromString(const char *s) {
  Superstring *ss = superstringCreate(strlen(s));
  strcpy(ss->data,s);

  return ss;
}

Superstring * superstringFromFile(FILE *f) {
  //printf("Start\n");
  
  nodeq *q = malloc(sizeof(nodeq));
  qInit(q);
  node* n;
  int c;
  size_t size = 0,i=0;
  while( (c=getc(f)) != EOF ) {
    size++;
    n = malloc(sizeof(node));
    nodeInit(n,c);
    qAdd(q,n);
  }

  Superstring *s=superstringCreate(size);

  n = q->head;
  while(n!=NULL) {
    s->data[i] = n->c;
    n = n->next;
    i++;
  }

  freeQNodes(q);
  free(q);

  /*
  char buf[1001];
  buf[1000] = '\0';
  int c,i=0,start=0,j,curlen=0;
  size_t size=0;
  Superstring *s=NULL;
  Superstring *temp;
  while( (c=getc(f)) != EOF ) {
    //printf("Hi\n");
    size++;
    buf[i] = c;
    i++;
    if (i >= 1000) {
      temp = superstringCreate(size);
      for (j=0;j<curlen;j++)
        temp->data[j] = s->data[j];
      for (j=0;j<1000;j++) {
        temp->data[start]=buf[j];
        start++;
      }
      superstringDestroy(s);
      s = temp;
      curlen = s->len;
      i = 0;
    }
  }

  if(size == 0) {
    superstringDestroy(s);
    return 0;
  }

  temp = superstringCreate(size);
  for (j=0;j<curlen;j++)
    temp->data[j] = s->data[j];
  for (j=0;j<i;j++) {
    temp->data[start]=buf[j];
    start++;
  }
  superstringDestroy(s);
  s = temp;
  curlen = s->len;
  */

  return s;
}
size_t superstringSize(const Superstring *s) {
  return (size_t) s->len;
}

int superstringSet(Superstring *s, size_t position, int value) {
  if(position >= s->len)
    return 0;
  s->data[position] = value;
  return 1;
}

const char* superstringAsString(const Superstring *s) {
  return ((const char*) s->data);
}

const char* superstringUpcased(const Superstring *s) {
  int i;
  for (i=0;i<=s->len;i++) {
    s->upcased[i] = toupper(s->data[i]);
  }
  return (const char *) s->upcased;
}

