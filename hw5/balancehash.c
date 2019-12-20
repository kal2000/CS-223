#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

typedef struct node_t {
  int c;
  int pos;
  int line;
  struct node_t* next;
}node;

void nodeInit(node* n, int nc, int np,int nl) {
  n->c = nc;
  n->next=NULL;
  n->pos = np;
  n->line = nl;
}

typedef struct mystack {
  node* head;
} stack;

void sInit(stack *s) {
  s->head = NULL;
}

void sAdd(stack*s, node* n) {
  if(s->head==NULL) {
    s->head=n;
  }
  else {
    n->next = s->head;
    s->head = n;
  }
}

void sAddInit(stack *s,int c,int p,int l) {
  node* n = malloc(sizeof(node));
  nodeInit(n,c,p,l);
  sAdd(s,n);   
}

node* sPop(stack* s){
  if(s->head==NULL)
    return NULL;
  node* temp = s->head;
  s->head = s->head->next;
  temp->next = NULL;
  return temp;
}

int sPeek(stack *s) {
  if (s!= NULL)
    return s->head->c;
  else
    return -5;
}

int sEmpty(stack*s) {
  if(s->head == NULL)
    return 1;
  else
    return 0;
}

void freeStack (stack*s) {
  node* temp=s->head;
  while(temp!=NULL) {
    s->head=s->head->next;
    free(temp);
    temp=s->head;
  }
}

void printErr(stack *s) {
  node *temp = s->head;
  while(temp!=NULL) {
    if(temp->c==EOF)
      printf("%d:%d:EOF\n",temp->line,temp->pos);
    else
      printf("%d:%d:%c\n",temp->line,temp->pos,temp->c);
    s->head = s->head->next;
    free(temp);
    temp = s->head;
  }
}

/*
int match(char *a, char*b, int len_a, int len_b) {
  int i,j;
  for (i=0;i<len_a;i++){
    for (j=0;j<len_b;j++) {
      if (a[i] == b[j])
        return 1;
    }
  }
  return 0;
}

int unique(char *a, int len) {
  int i,j;
  for (i=0;i<len;i++) {
    for (j=i+1;j<len;j++)
      if (a[i] == a[j])
        return 0;
  }

  return 1;
}
*/

int main(int argc, char** argv) {
  int i,c,len=0;
  if (argc<2)
    return 0;

  while(argv[1][len] != '\0')
    len++;
  if (len%2!=0) {
    return 1;
  } // make sure length is even

  int openh[UCHAR_MAX+5][2], closeh[UCHAR_MAX+5][2];
  for (i=0;i<UCHAR_MAX+5;i++) {
    openh[i][0] = 0;
    closeh[i][0] = 0;
  } //openh[ ASCII ] [ T/F, matching char]

  char open[len/2], close[len/2];
  for (i=0;i<len;i++) {
    if(i%2 == 0) {
      open[i/2]=argv[1][i];
      openh[(int)argv[1][i]][0]++;
      if (openh[(int)argv[1][i]][0] > 1)
        return 2;
    }
    else {
      close[i/2]=argv[1][i];
      closeh[(int)argv[1][i]][0]++;
      openh[(int)argv[1][i-1]][1] = argv[1][i];
      closeh[(int)argv[1][i]][1] = argv[1][i-1];
      if (closeh[(int)argv[1][i]][0] > 1)
        return 2;
    }
  } //list of open and close pairs
  //now have hashes with 1's if a character is in open or close

  stack *s = malloc(sizeof(stack));
  sInit(s);
  node* temp;

/*
  for (i=0;i<len;i++){
    printf("%c%c",open[i],close[i]);
  }
  printf("\n");
*/

  int lines=1,curpos=1,popped;
  //lines keeps track of newlines. curpos keeps track of place in line
  while( (c = getchar())!=EOF ) {
    popped=0;
    if (closeh[c][0]) { //character is a closer
      if ( !(sEmpty(s)) && sPeek(s) == closeh[c][1] ) { //matches the last opener
        temp = sPop(s);
        free(temp);
        popped = 1;
      }
      else if (!openh[c][0]){ //unmatched closer that is not an opener
        sAddInit(s,c,curpos,lines);
        printErr(s);
        freeStack(s);
        free(s);
        return 3; //unmatched closer
        }
    }
    if (openh[c][0] && !popped) { //character is an opener
    //character is not also a closer that matched the last opener
      sAddInit(s,c,curpos,lines);
    }

    curpos++;
    if (c=='\n') {
      lines++;
      curpos = 1;
    }
  }

  if (!sEmpty(s)) { //some unmatched openers after exiting loop
    sAddInit(s,c,curpos,lines);
    printErr(s);
    freeStack(s);
    free(s);
    return 4; //EOF too early
  }

  //printf("\nNo problems\n");

  freeStack(s);
  free(s);

  return 0;
}
