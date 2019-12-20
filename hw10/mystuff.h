typedef struct position pos;
typedef long long lint;
typedef struct node_t node;
typedef struct heap_t heap;

int h(pos p1, pos p2);
void swap(node* n1,node* n2);
int hEmpty(heap *h);
node* nodeCreate(pos p, pos t, int dist);
void heapifyUp(heap* h,int i);
void heapPush(heap* h, node* ptr);
void heapifyDown(heap* h,int i);
node* heapPop(heap *h);
heap* heapCreate(size_t s);
void heapDestroy(heap *h);
int posEq(pos p1, pos p2);
void printHeap(heap *h);
int hNumels(heap *h);
