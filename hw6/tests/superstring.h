/* this prevents problems with multiple inclusion */
#ifndef __SUPERSTRING_H
#define __SUPERSTRING_H 1

#include <stdio.h>

typedef struct superstring Superstring;

/* create a new superstring object initialized to all-nul
 * that can hold up to size characters plus a final nul
 */
Superstring *
superstringCreate(size_t size);

/* free a superstring and any related data */
void 
superstringDestroy(Superstring *s);

/* create a new superstring with given initial contents */
/* size should be equal to strlen(s) */
Superstring *
superstringFromString(const char *s);

/* create a new superstring whose contents are read from a file */
/* size will be number of characters read before EOF */
Superstring *
superstringFromFile(FILE *f);

/* return the size of a superstring
 * this will equal the argument passed to superstringCreate */
size_t
superstringSize(const Superstring *s);

/* set a byte of a superstring; returns 1 if position is in range */
/* if position is out of range, do nothing and return 0 */
int
superstringSet(Superstring *s, size_t position, int value);

/* return contents of a superstring as a string */
const char *
superstringAsString(const Superstring *s);

/* return upper-cased version of superstring
 * e.g. puts(superstringUpcased(superstringFromString("abc")))
 * will print "ABC".
 *
 * The user does *not* need to free the returned string,
 * which should remain usable at least until the next
 * time the superstring is modified.
 */
const char *
superstringUpcased(const Superstring *s);

#endif
