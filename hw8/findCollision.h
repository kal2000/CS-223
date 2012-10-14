/* finds values x1 != x2 such that f(x1) & 0x7fffffff == f(x2) & 0x7fffffff */
void findCollision(unsigned int (*f)(unsigned int), unsigned int *x1, unsigned int *x2);
