#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "superstring.h"

struct {
    int testsDone;
    int testsPassed;
} testCounter;

#define Test(assertion) \
    (((assertion) ? testCounter.testsPassed++ : \
        fprintf(stderr, "%s:%d: test failed: " # assertion "\n", __FILE__, __LINE__)),\
     testCounter.testsDone++)

#define BIG_SIZE (1000000)  /* size of big string */

#define TEST_FILE_NAME "test_superstring.tempfile" /* where to put the test file */

int
main(int argc, char **argv)
{
    Superstring *buffer;    /* we'll fill this by hand */
    Superstring *hello;     /* we'll build this from a string */
    Superstring *big;       /* big superstring for speed test */
    int i;                  /* loop index for filling big */
    const char *big_value;  /* values from big */
    int worked;             /* flag for consolidating big string tests */
    Superstring *fromFile;  /* string from file */
    FILE *f;                /* file descriptor for file operations */
    Superstring *bigFile;   /* big superstring from file */

    hello = superstringFromString("hello world");

    Test(hello != 0);
    Test(superstringSize(hello) == strlen("hello world"));
    Test(superstringAsString(hello) != 0);
    Test(!strcmp(superstringAsString(hello), "hello world"));
    Test(!strcmp(superstringUpcased(hello), "HELLO WORLD"));

    Test(superstringSet(hello, 5, '-') == 1);
    Test(superstringSet(hello, 12, '-') == 0);
    Test(!strcmp(superstringAsString(hello), "hello-world"));
    Test(!strcmp(superstringUpcased(hello), "HELLO-WORLD"));

    buffer = superstringCreate(2);
    Test(buffer != 0);
    Test(superstringSize(buffer) == 2);
    Test(superstringAsString(buffer)[0] == '\0');
    Test(superstringSet(buffer, 1, 'a') == 1);
    Test(!strcmp(superstringAsString(buffer), ""));
    Test(superstringSet(buffer, 2, 'd') == 0);
    Test(!strcmp(superstringAsString(buffer), ""));
    Test(superstringSet(buffer, 0, 'b') == 1);
    Test(!strcmp(superstringAsString(buffer), "ba"));
    Test(!strcmp(superstringUpcased(buffer), "BA"));
    Test(superstringSet(buffer, 1, '\0') == 1);
    Test(!strcmp(superstringAsString(buffer), "b"));
    Test(!strcmp(superstringUpcased(buffer), "B"));
    Test(superstringSize(buffer) == 2);

    big = superstringCreate(BIG_SIZE);
    Test(big != 0);

    Test(!strcmp(superstringAsString(big), ""));
    Test(!strcmp(superstringUpcased(big), ""));

    /* fill big */
    worked = 1;
    for(i = 0; i < BIG_SIZE; i++) {
        /* suprisingly, C does not have &&= */
        worked = worked && (superstringSet(big, i, 'q') == 1);
    }
    Test(worked);

    Test(superstringSet(big, BIG_SIZE, 'q') == 0);

    Test(superstringSize(big) == BIG_SIZE);

    /* check big */
    big_value = superstringAsString(big);
    Test(big_value != 0);

    worked = 1;
    for(i = 0; i < BIG_SIZE; i++) {
        worked = worked && (big_value[i] == 'q');
    }
    Test(worked);

    Test(big_value[BIG_SIZE] == '\0');

    /* check upcased big */
    big_value = superstringUpcased(big);
    Test(big_value != 0);

    worked = 1;
    for(i = 0; i < BIG_SIZE; i++) {
        worked = worked && (big_value[i] == 'Q');
    }
    Test(worked);

    Test(big_value[BIG_SIZE] == '\0');

    /* build file to read from */
    f = fopen(TEST_FILE_NAME, "wb");
    assert(f);  /* not really a test */
    fputs("this\nis", f);
    putc('\0', f);
    fputs("a\ntest\n", f);
    fclose(f);

    /* read it back in */
    f = fopen(TEST_FILE_NAME, "rb");
    fromFile = superstringFromFile(f);
    fclose(f);

    Test(fromFile != 0);
    Test(superstringSize(fromFile) == 15);

    Test(!strcmp(superstringAsString(fromFile), "this\nis"));
    Test(superstringAsString(fromFile)[7] == '\0');
    Test(!strcmp(superstringAsString(fromFile)+8, "a\ntest\n"));

    Test(!strcmp(superstringUpcased(fromFile), "THIS\nIS"));
    Test(superstringUpcased(fromFile)[7] == '\0');
    Test(!strcmp(superstringUpcased(fromFile)+8, "A\nTEST\n"));

    Test(superstringSet(fromFile, 7, '\n') == 1);
    Test(!strcmp(superstringAsString(fromFile), "this\nis\na\ntest\n"));
    Test(!strcmp(superstringUpcased(fromFile), "THIS\nIS\nA\nTEST\n"));

    /* build another file to read from */
    f = fopen(TEST_FILE_NAME, "wb");
    assert(f);  /* not really a test */
    for(i = 0; i < BIG_SIZE; i++) {
        putc('z', f);
    }
    fclose(f);

    /* read it back in */
    f = fopen(TEST_FILE_NAME, "rb");
    bigFile = superstringFromFile(f);
    fclose(f);

    Test(superstringSet(bigFile, BIG_SIZE, 'z') == 0);

    Test(superstringSize(bigFile) == BIG_SIZE);

    /* check bigFile */
    big_value = superstringAsString(bigFile);
    Test(big_value != 0);

    worked = 1;
    for(i = 0; i < BIG_SIZE; i++) {
        worked = worked && (big_value[i] == 'z');
    }
    Test(worked);

    Test(big_value[BIG_SIZE] == '\0');

    /* check upcased bigFile */
    big_value = superstringUpcased(bigFile);
    Test(big_value != 0);

    worked = 1;
    for(i = 0; i < BIG_SIZE; i++) {
        worked = worked && (big_value[i] == 'Z');
    }
    Test(worked);

    Test(big_value[BIG_SIZE] == '\0');
    /* clean up after ourselves */
    unlink(TEST_FILE_NAME);

    superstringDestroy(buffer);
    superstringDestroy(hello);
    superstringDestroy(big);
    superstringDestroy(fromFile);
    superstringDestroy(bigFile);

    fprintf(stderr, "%d/%d tests passed\n", testCounter.testsPassed, testCounter.testsDone);

    return testCounter.testsPassed != testCounter.testsDone;
}
