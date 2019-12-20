typedef struct dict *Dict;

/* create a new empty dictionary */
Dict DictCreate(void);

/* destroy a dictionary */
void DictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void DictInsert(Dict, int key, struct position value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
int DictSearch(Dict, int key, struct position value);

