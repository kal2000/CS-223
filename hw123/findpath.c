#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

typedef struct node_t {
  int x;
  int y;
  int d;
  struct node_t* next;
  struct node_t* pathPrev;
}node;

void nodeInit(node* n, int nx, int ny, int nd,node* prev) {
  n->d = nd;
  n->next=NULL;
  n->x = nx;
  n->y = ny;
  n->pathPrev = prev;
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

void qPathAdd(nodeq* q, node* n){
  if(q->head==NULL) {
    q->head=n;
    q->tail=n;
  }
  else {
    q->tail->pathPrev = n;
    q->tail = n;
  }
}
/*
node* copyNode(node* n) {
  node* copy = malloc(sizeof(node));
  assert(copy!=NULL);
  copy->x = n->x;
  copy->y = n->y;
  copy->d = n->d;
  copy->next = n->next;
  copy->pathPrev = n->pathPrev;
  return copy;
}*/

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

node* qPathPop(nodeq* q){
  if(q->head==NULL)
    return NULL;
  node* temp = q->head;
  q->head = q->head->pathPrev;
  if(q->head == NULL)
    q->tail = NULL;
  return temp;
}


int qEmpty(nodeq* q) {
  if(q->head == NULL)
    return 1;
  else
    return 0;
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

void printQ(nodeq* q) {
  node* temp = q->head;
  while(temp!=NULL) {
    printf("%d %d\n",temp->x,temp->y);
    temp = temp->next;
  }
}

void genPath(nodeq* q, node* n) {
  node* temp = n;
  do {
    //temp = copyNode(temp);
    //printf("Added %d %d\n",temp->x,temp->y);
    qPathAdd(q,temp);
    temp = temp->pathPrev;
  } while(temp!=NULL);
}

void drawPath(int r, int c, char map[r][c], nodeq* path) {
  node* temp = qPathPop(path);
  int x,y;
  while(temp!=NULL) {
    //printf("%d %d\n",temp->x,temp->y);
    x = temp->x;
    y = temp->y;
    if(map[x][y]!='$'&&map[x][y]!='@')//avoid start and end
    //won't have more than one $ and @ in a path
      map[x][y]='*';
    temp = qPathPop(path);
  }
}

//adds unvisited neighbors to queue
node* getNeighb(node* cNode, int r, int c,
               char map[r][c], int dist[r][c], nodeq* q) {
  int i,j;
  int x=cNode->x;
  int y=cNode->y;
  int d=cNode->d;
  
  node* temp;
  //printf("Visiting %d %d: d=%d\n",x,y,d);

  for(i=x-1;i<=x+1;i++) {
    if(i<0||i>=r)
      continue;
    for(j=y-1;j<=y+1;j++) {
      if(j<0||j>=c)
        continue;
      //printf("%d  %d\n",i,j);
      //now we know we are in a valid place in map

      //check if unvisited and can visit, add it to the queue
      if((map[i][j]==' '||map[i][j]=='$') && dist[i][j]==INT_MAX) {
        temp = malloc(sizeof(node));
        assert(temp!=NULL);
        nodeInit(temp,i,j,d+1,cNode);
        qAdd(q,temp);
        dist[i][j]= d+1; //mark as visited with the distance
        if(map[i][j]=='$')
          return temp; //return temp ONLY if a finish is found
      }
      //else don't do anything (obstacle, already visited)
    }
  }
  return NULL;
}

void printCharArr(int r, int c, char arr[r][c]) {
  int i,j;
  for(i=0;i<r;i++) {
    for(j=0;j<c;j++)
      printf("%c",arr[i][j]);
    printf("\n");
  }
}

void printIntArr(int r, int c, int arr[r][c]) {
  int i,j;
  for(i=0;i<r;i++) {
    for(j=0;j<c;j++)
      if(arr[i][j]==INT_MAX)
        printf(" inf ");
      else
        printf(" %3i ",arr[i][j]);
    printf("\n");
  }
}

int main() {
  int h,w,i,j,c;
  int starts=0,dests=0;
  node* temp;

  scanf("%d %d",&h,&w);
  c = getchar();

  if (h<0||w<0) {
    fprintf(stderr,"Bad width and/or height fields of map \n");
    return 1;
  }
  if (h==0) {
    return 0;
  }
  if (w==0) {
    for(i=0;i<h;i++)
      printf("\n");
    return 0;
  }

  char map[h][w];
  int dist[h][w];

  //set initial distances to inf
  for(i=0;i<h;i++)
    for(j=0;j<w;j++) {
      dist[i][j]=INT_MAX;
    }

  nodeq* neighbQ = malloc(sizeof(nodeq));
  qInit(neighbQ);
  nodeq* visited = malloc(sizeof(nodeq));
  qInit(visited);
  nodeq* path = malloc(sizeof(nodeq));
  qInit(path);

  assert(neighbQ!=NULL&&visited!=NULL&&path!=NULL);

  //process the input
  for(i=0;i<h;i++) {
    for(j=0;j<w;j++) {
      c = getchar();
      //printf("%c",c);
      if (c==EOF) {
        fprintf(stderr,
        "EOF found earlier than expected in row %d and column %d \n",
        i,j);
        return 2;
      }
      if (c=='\n') {
        fprintf(stderr,
        "Newline found earlier than expected in row %d and column %d\n",
        i,j);
        return 3;
      }

      if (c == '@') {
        starts++;
        dist[i][j]=0;

        //printf("%d %d\n",i,j);
        node* snode = malloc(sizeof(node));
        assert(snode!=NULL);
        nodeInit(snode,i,j,0,NULL);
        qAdd(neighbQ,snode);

        }
      else if (c == '$')
        dests++;
      map[i][j] = c;
    }

    c = getchar();
    if(c==EOF) {
      fprintf(stderr,"Expected newline in row %d, found EOF",i);
      return 2;
    }
    if(c!='\n') {
      fprintf(stderr,"Line too long, expected newline in row %d",i);
      return 3;
    }
  }

  if(starts==0||dests==0) {
    printCharArr(h,w,map);
    return 0;
  }

  //printf("\n");

  //get neighbors for each node in the queue until a finish is found
  while(qEmpty(neighbQ)==0) {
    temp = qPop(neighbQ);
    //printf("%d %d\n",temp->x,temp->y);
    qAdd(visited,temp);
    temp = getNeighb(temp,h,w,map,dist,neighbQ);
    if(temp!=NULL)
      break;
    //printIntArr(h,w,dist);
  }
  if(temp!=NULL) {
    //temp = copyNode(temp); //create a copy of temp in a new location
    //printf("Shortest path found\n");
    genPath(path,temp);
    //printf("Shortest path generated\n");
    //printQ(path);

    drawPath(h,w,map,path);
    //printf("Shortest path drawn\n");
  }
  //else
  //  printf("No path found\n");
  //printQ(neighbQ);

  printCharArr(h,w,map);

  //printQ(neighbQ);
  //printQ(visited);

  freeQNodes(neighbQ);
  freeQNodes(visited);
  freeQNodes(path);
  
  assert(qEmpty(neighbQ)==1&&qEmpty(visited)==1&&qEmpty(path)==1);
  free(neighbQ);
  free(visited);
  free(path);

  //printf("\n");
  //printIntArr(h,w,dist);

  return 0;
}
