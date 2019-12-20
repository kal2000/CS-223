#include <stdio.h>

typedef struct charhash {
  const char** data;
  int size;
} chash;

void hInit(int s, chash *h) {
  h->data = malloc(sizeof(const char*) * s);
  h->size = s;
}

unsigned long hash(const char *s)
{
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) s;

    h = 0;
    while(*us != '\0') {
        h = h * 37 + *us;
        us++;
    } 

    return h;
}




int main() {

  return 0;
}

