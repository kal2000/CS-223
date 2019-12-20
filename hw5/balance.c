#include <stdio.h>
#include "balance.h"
#include <string.h>

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

int main(int argc, char** argv) {
  int i;
  int len=0; //get length
  
  if (argc<2)
    return 0; //TAKE CARE OF NO MATCHER CASE

  while(argv[1][len] != '\0') {
    len++;
  }

  if (len%2!=0) {
    return 1;
  } // make sure length is even

  char open[len/2], close[len/2];
  for (i=0;i<len;i++) {
    if(i%2 == 0) {
      open[i/2]=argv[1][i];
    }
    else {
      close[i/2]=argv[1][i];
    }
  }
  
  if(!(unique(open,len/2) && unique(close,len/2)))
    return 2; //open and close must have unique chars

  printf("match %d\n",match(open,close,len/2,len/2));



  return 0;
}
