#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getbest(int min, int max, int curind, int *arr, int *best) {
  int i,curbest,bestind,found=0,edge=0;
  for(i=curind-max-1;i<curind-min;i++) {
    if (i<0) {edge = 1; continue;}
    else if (!found || best[i]>curbest) {
      found = 1;
      curbest = best[i];
      bestind = i;
      }
  }
  //ignore if only negative houses before and are at edge or if didn't find any houses before
  if(found) {
    if(edge && curbest < 0) bestind = -1;
  }
  else {
    bestind = -1; //fprintf(stderr,"not found\n");
  }

  return bestind;
}

int main(int argc, int **argv) {
  const int INITIAL_SIZE = 1;
  int size = INITIAL_SIZE;
  int min = atoi( (const char*) argv[1]), max = atoi( (const char*) argv[2]);
  int *inp, *best, *prev, *out, i, c, count=0, temp;

  inp = malloc(sizeof(int)*size);

  i=0;
  while ( scanf("%d",&c) != EOF ) {
//    printf("%d\n",c);
    inp[i] = c;
    count++; i++;
    if (i>3*size/4) {
      size *= 2;
      inp = realloc(inp, sizeof(int)*size);
    }
  }
  best = malloc(sizeof(int)*size);
  prev = malloc(sizeof(int)*size);
  out = malloc(sizeof(int)*size);

//  fprintf(stderr,"%d els\n",count);

  for(i=0;i<count;i++) {
    temp = getbest(min,max,i,inp,best);
    if (temp == -1) {
      best[i] = inp[i];
      prev[i] = -1;
    }
    else {
      best[i] = best[temp] + inp[i];
      prev[i] = temp;
    }
//    fprintf(stderr,"%2d %3d %3d\n",i,temp,best[i]);
  }

  int tempbest,tempind,found=0;
  for (i=count-max-1;i<count;i++) {
    if (i<0) continue;
    else if (!found || best[i]>tempbest) {
      found = 1;
      tempbest = best[i];
      tempind = i;
    }
  }
  printf("%d\n",best[tempind]);
  for (i=0;i<count;i++) {
//    fprintf(stderr,"%d | ",best[i]);
    out[i] = 0;
  }
// fprintf(stderr,"\n");
  temp = tempind;
  do {
    out[temp] = 1;
    temp = prev[temp];
  } while (temp != -1);

  for (i=0;i<count;i++) {
    printf("%d",out[i]);
  }
  printf("\n");

  free(prev);
  free(inp);
  free(best);
  return 0;
}
