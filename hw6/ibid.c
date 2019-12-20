#include <stdio.h>
#include "superstring.h"
#include <string.h>
#include <stdlib.h>

typedef struct nodestruct {
  const char* data;
  const char* line;
  struct nodestruct* next;
} node;

void nodeInit(node* n, const char* s, const char* l) {
  n->next = NULL;
  n->data = s;
  n->line = l;
}

typedef struct linkedlist {
  node* head;
} LL;

void LLInit(LL* x){
  x->head = NULL;
}

int lEmpty(LL*L) {
  if(L->head == NULL)
    return 1;
  else
    return 0;
}

void lAdd(LL*L, node* n) {
  if(L->head==NULL) {
    L->head=n;
  }
  else {
    n->next = L->head;
    L->head = n;
  }
}

node* lsearch(LL* L, const char* s) {
  node* temp = L->head;

  while(temp != NULL) {
    if (strcmp(temp->data,s) == 0) {
      return temp;
    }
    temp = temp->next;
  }

  return NULL;
}

void freeList (LL*L) {
  node* temp=L->head;
  while(temp!=NULL) {
    //fprintf(stderr,"freeing %s|| next = %p\n",temp->data,temp->next);
    L->head=L->head->next;
    if (temp->data != NULL) free((char*)temp->data);
    if (temp->line != NULL) free((char*)temp->line);
    free(temp);
    temp=L->head;
  }
}

void printList (LL*L) {
  node* temp=L->head;
  while(temp!=NULL) {
    printf("%s ",temp->data);
    temp=temp->next;
  }
  printf("\n");
}

//Make a string from [start,end) of a character array
const char * makeString(const char *start, const char* end) {
  char *s = malloc(sizeof(char) * (end-start+1));
  size_t i;
  for(i=0;i<end-start;i++) {
    s[i] = start[i];
  }
  s[i] = '\0';
  return (const char*) s;
}

int main() {
  Superstring *s = superstringFromFile(stdin);
  int i,linestart,foundauthor,firstline,firstcomma,opcit;
  const char* input = superstringAsString(s);
  const char* temp;
  const char* lastLine;
  const char* thisLine;
  node* n;
  node* thisauthor;

  /*for(i=0;i<superstringSize(s);i++) {
    printf("%c",input[i]);
  }
  printf("\n");*/

  LL* authors = malloc(sizeof(LL));
  LLInit(authors);
  LL* sparelines = malloc(sizeof(LL));
  LLInit(sparelines);

  i = 0; firstline = 1; firstcomma = -1;
  while(1) {
    linestart = i; foundauthor = 0; opcit = 0;
    while (input[i] != '\n' && input[i] != EOF) {
      //printf("%c:%d\n",input[i],i);
      if (input[i] == ',' && foundauthor == 0) {
        firstcomma = i;
        foundauthor = 1;
      }
      i++;
    } //exit on newline (or premature EOF)
    if (input[i] == EOF) break;
    //input[i] == '\n'

    thisLine = makeString(input+linestart,input+i);
    //get author
    if (firstcomma >= linestart)
      temp = makeString(input+linestart,input+firstcomma);
    else
      temp = makeString(input+linestart,input+i);

    n = malloc(sizeof(node));
    nodeInit(n,temp,thisLine);

    if (firstline) {
      lastLine = thisLine;
      printf("%s\n",thisLine);
      firstline=0;
      lAdd(authors,n);      
    }
    else {
      if (strcmp(thisLine,lastLine) == 0) {
        printf("ibid.\n");
        lAdd(sparelines,n);  
      }
      else {
        thisauthor = lsearch(authors,temp);
        if ( thisauthor == NULL) { //new author
          lAdd(authors,n);
        }
        else { //check if this author's last line was the same
          //last line was the same
          if(strcmp(thisauthor->line,thisLine)==0) {
            printf("op. cit. %s\n",thisauthor->data);
            opcit = 1;
            //fprintf(stderr,"<%p %s>",n,temp);
            lAdd(sparelines,n);      
          }
          //last line wasn't the same
          else {
            nodeInit(n,temp,thisauthor->line);
            thisauthor->line = thisLine;
            lAdd(sparelines,n);
          }
        }

        if (opcit == 0)
          printf("%s\n",thisLine);
      }
      
      lastLine = thisLine;
    }

    i++;
  }

  //printf("\n\n");
  //printList(authors);

  freeList(authors);
  //fprintf(stderr,"\n");
  freeList(sparelines);
  free(authors);
  free(sparelines);
  superstringDestroy(s);
  //if (lastLine != NULL) free((char*)lastLine);
  return 0;
}
