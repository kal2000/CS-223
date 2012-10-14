#include <stdio.h>
#include <ctype.h>
#include <limits.h>


int main()
{
  int c,i;
  unsigned int count[UCHAR_MAX+5];
  for(i=0;i<UCHAR_MAX+5;i++)
    count[i] = 0u;
  while( (c = getchar()) != EOF) {
    count[c]++;
  }

  for(i=0;i<UCHAR_MAX+5;i++) {
    if (count[i] != 0) {
      if (isspace(i)|!isprint(i))
        printf("<%02x> %u\n",i,count[i]);
      else {
        printf("%c %u\n",i,count[i]);
      }
    }
  }

  return 0;
}
