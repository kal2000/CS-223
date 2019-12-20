#include <stdio.h>
#include "superstring.h"
#include <string.h>
#include <stdlib.h>
#include "dict.h"

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

void freeNode(node* n) {
  if(n!= NULL) {
    if(n->data!=NULL) free((char*)n->data);
    if(n->line!=NULL) free((char*)n->line);
    free(n);
  }
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

void freeList (LL*L) {
  node* temp=L->head;
  while(temp!=NULL) {
    //fprintf(stderr,"freeing %s|| next = %p\n",temp->data,temp->next);
    L->head=L->head->next;
    freeNode(temp);
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
  struct elt* thisauthor;
  node* n;

  /*for(i=0;i<superstringSize(s);i++) {
    printf("%c",input[i]);
  }
  printf("\n");*/

  Dict authordict = DictCreate();
  LL* skipped = malloc(sizeof(LL));
  LLInit(skipped);

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

    n=malloc(sizeof(node));
    nodeInit(n,temp,thisLine);

    if (firstline) {
      lastLine = thisLine;
      printf("%s\n",thisLine);
      firstline=0;
      DictInsert(authordict,temp,thisLine);
      lAdd(skipped,n);
    }
    else {
      if (strcmp(thisLine,lastLine) == 0) {
        printf("ibid.\n");
        lAdd(skipped,n);
      }
      else {
        thisauthor = DictSearch(authordict,temp);
        if ( thisauthor == 0) { //new author
          DictInsert(authordict,temp,thisLine);
          lAdd(skipped,n);
        }
        else { //check if this author's last line was the same
          //last line was the same
          if(strcmp(thisauthor->value,thisLine)==0) {
            printf("op. cit. %s\n",temp);
            opcit = 1;
            lAdd(skipped,n);

            //fprintf(stderr,"<%p %s>",n,temp);    
          }
          //last line wasn't the same
          else {
            nodeInit(n,temp,thisauthor->value);
            thisauthor->value = (char*)thisLine;
            lAdd(skipped,n);
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

  DictDestroy(authordict);

  //fprintf(stderr,"\n");
  freeList(skipped);
  free(skipped);

  superstringDestroy(s);
  //if (lastLine != NULL) free((char*)lastLine);
  return 0;
}
