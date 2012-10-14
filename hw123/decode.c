#include <stdio.h>

void decode(int *num) {
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

int main() {
  int c, i;
  int lineDone;
  i = 0;
  int line[9];
  lineDone = 0;

  while(1) { //process each line
    c = getchar();
    if (c == EOF)
      break;

    if (lineDone) { //if output line is complete
      if (c == '\n') { //check for newline, else keep reading
        i = 0;
        lineDone = 0;
        decode(line);
      }
      continue;
    }

    i++; //i tells you how many chars you have read

    if ( (c >= 'a' && c <= 'j') || (c >= 'A' && c <= 'J') )
      line[i]=1;
    else
      line[i]=0;

    if (c == '\n') { //finish current line
      while(i<8) {
        i++;
        line[i] = 0;
      }
      decode(line);
      i = 0;
    }

    if (i==8) {
      lineDone = 1;
    }
  }

  //putchar('\n');
  return 0;
}

