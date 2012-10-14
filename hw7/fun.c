#include <stdio.h>
#include <stdlib.h>
#include "fun.h"

#define CONSTANT (0)
#define FUNCTION (1)
#define TABLE (2)
#define COMPOSITION (3)

/* a Fun represents a function from ints to ints */
struct fun {
  int (*f)(int x);
  const Fun *f1;
  const Fun *f2;
  int *invals;
  int *outvals;
  int tableLen;
  int c;
  int t;
};

//Search in [start,end)
int bSearch(int val, int *arr, int start, int end) {
  if (start == end) {
    if( arr[start] < val) return start;
    else return start-1;
  }
  if (start > end) return -1;
  int temp = (start+end)/2;
  if (val == arr[temp]) return temp;
  else if (val < arr[temp]) return bSearch(val, arr, start, temp);
  else return bSearch(val, arr, temp+1, end);
}

int findIn(int val, int n, int *arr) {
  if (val < arr[0]) return 0;
  if (val >= arr[n-1]) return n-1;
  return bSearch(val, arr, 0, n);
}

/* this routine calls a Fun and returns its value */
int funCall(const Fun *func, int x) {
  switch (func->t) {
    case CONSTANT:
      return func->c;
    case FUNCTION:
      return func->f(x);
    case TABLE:
      return func->outvals[findIn(x,func->tableLen,func->invals)];
    case COMPOSITION:
      return funCall(func->f1, funCall(func->f2, x) );
    default:
      return -1;
  }
}

/* build a Fun whose value is constant */
Fun *funConstant(int x) {
  Fun *func = malloc (sizeof(Fun));
  func->c = x;
  func->t = CONSTANT;
  func->invals = NULL;
  func->outvals = NULL;
  return func;
}


/* build a Fun based on an actual function */
Fun *funFromFunction(int (*inp)(int x)) {
  Fun *func = malloc (sizeof(Fun));
  func->f = inp;
  func->t = FUNCTION;
  func->invals = NULL;
  func->outvals = NULL;
  return func;
}

/* build a Fun f from a table */ 
/* arguments are length of arrays n */
/* increasing array of input values */
/* array of corresponding output values */
/* value of funCall(f, x) is:
 *   
 *   output[0] when   x < input[0]
 *   output[i] when   input[i] <= x < input[i+1]
 *   output[n-1] when x >= input[n-1]
 *
 */
/* funFromTable should copy input and output, so
 * that it continues to work even if they later change */
Fun *funFromTable(int n, const int *input, const int *output) {
  Fun *func = malloc(sizeof(Fun));
  func->tableLen = n;
  func->invals = malloc(sizeof(int) * n);
  func->outvals = malloc(sizeof(int) * n);
  int i;
  for (i=0;i<n;i++) {
    func->invals[i] = input[i];
    func->outvals[i] = output[i];
  }
  func->t = TABLE;
  return func;
}

/* build a Fun by composing two Funs */
/* value at x is f(g(x)) */
/* does NOT copy f or g */
Fun *funCompose(const Fun *func1, const Fun *func2) {
  Fun *func = malloc(sizeof(Fun));
  func->f1 = func1;
  func->f2 = func2;
  func->t = COMPOSITION;
  func->invals = NULL;
  func->outvals = NULL;
  return func;
}

/* destroy a Fun, freeing any blocks allocated when it is created */
void funDestroy(Fun *func) {
  if(func==NULL) return;
  if (func->invals != NULL) free(func->invals);
  if (func->outvals != NULL) free(func->outvals);
  free(func);
}
