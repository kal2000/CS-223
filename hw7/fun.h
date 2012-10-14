/* a Fun represents a function from ints to ints */
typedef struct fun Fun;

/* this routine calls a Fun and returns its value */
int funCall(const Fun *, int);

/* build a Fun whose value is constant */
Fun *funConstant(int);

/* build a Fun based on an actual function */
Fun *funFromFunction(int (*)(int));

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
Fun *funFromTable(int n, const int *input, const int *output);

/* build a Fun by composing two Funs */
/* value at x is f(g(x)) */
/* does NOT copy f or g */
Fun *funCompose(const Fun *f, const Fun *g);

/* destroy a Fun, freeing any blocks allocated when it is created */
void funDestroy(Fun *);
