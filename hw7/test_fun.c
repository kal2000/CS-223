#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "fun.h"

static int add1(int x) { return x+1; }

#define BIG (100000)

int
main(int argc, char **argv)
{
    Fun *zero;
    Fun *plus1;
    Fun *one;
    int *input;
    int *output;
    int *outputCopy;
    int i;
    Fun *table;
    int log3_input[]  = { 1, 3, 9, 27, 81, 243, 729 };
    int log3_output[] = { 0, 1, 2,  3,  4,   5,   6 };
    Fun *log3;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    zero = funConstant(0);

    assert(funCall(zero, 0) == 0);
    assert(funCall(zero, -1237) == 0);
    assert(funCall(zero, 32191) == 0);

    plus1 = funFromFunction(add1);

    assert(funCall(plus1, 0) == 1);
    assert(funCall(plus1, -11917) == -11916);
    assert(funCall(plus1, 29921) == 29922);

    one = funCompose(plus1, zero);

    assert(funCall(one, 0) == 1);
    assert(funCall(one, -19912) == 1);
    assert(funCall(one, 191295) == 1);

    /* use fixed seed for consistency between runs */
    srand(0xf18d5eed);

    input = malloc(BIG * sizeof(int));
    output = malloc(BIG * sizeof(int));
    outputCopy = malloc(BIG * sizeof(int));

    for(i = 0; i < BIG; i++) {
        input[i] = 2*i;
        output[i] = rand();
        outputCopy[i] = output[i];
    }

    table = funFromTable(BIG, input, outputCopy);

    /* make sure input and outputCopy are really being copied */
    free(input);
    free(outputCopy);

    /* extreme values */
    assert(funCall(table, -5) == output[0]);
    assert(funCall(table, 17*BIG + 12) == output[BIG-1]);

    /* stress test */
    for(i = 0; i < BIG; i++) {
        assert(funCall(table, 2*i) == output[i]);
        assert(funCall(table, 2*i+1) == output[i]);
    }

    free(output);

    /* more search testing */
    log3 = funFromTable(7, log3_input, log3_output);

    assert(funCall(log3, -17) == 0);
    assert(funCall(log3, 112) == 4);
    assert(funCall(log3, 252) == 5);
    assert(funCall(log3, 1172) == 6);

    funDestroy(zero);
    funDestroy(plus1);
    funDestroy(one);
    funDestroy(table);
    funDestroy(log3);

    return 0;
}
