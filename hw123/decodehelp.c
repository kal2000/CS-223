#include <stdio.h>

int main() {
  int num[9];
  num
  int out,j,pow;
  out = 0;
  pow = 1;

  for(j=8;j>=1;j--) {
    out += num[j]*pow;
    pow *= 2;
  }

  printf("%c", out);

  return;
}


  return 0;
}
