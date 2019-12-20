#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "superstring.h"

struct superstring {
    size_t size;    /* size of this string */
    char *buffer;   /* contents; can hold size+1 bytes */
    char *upcased_buffer; /* upper-case version of buffer */
};

/* create a new superstring object initialized to all-nul
 * that can hold up to size characters plus a final nul.
 */
Superstring *
superstringCreate(size_t size)
{
    Superstring *s;

    s = malloc(sizeof(struct superstring));

    if(s == 0) {
        return 0;
    }

    s->size = size;
    s->buffer = calloc(size+1, 1);
    s->upcased_buffer = calloc(size+1, 1);

    if(s->buffer == 0 || s->upcased_buffer == 0) {
        superstringDestroy(s);
        return 0;
    }

    return(s);
}

/* free a superstring and any related data */
void 
superstringDestroy(Superstring *s)
{
    /* tests allows us to call this on incompletely-built superstrings */
    if(s==NULL) return;
    if(s->buffer) free(s->buffer);
    if(s->upcased_buffer) free(s->upcased_buffer);
    free(s);
}

/* create a new superstring with given initial contents */
/* size should be equal to strlen(s) */
Superstring *
superstringFromString(const char *s)
{
    Superstring *s2;
    int i;
    int len;

    len = strlen(s);

    s2 = superstringCreate(len);

    if(s2 == 0) {
        return 0;
    }

    /* using superstringSet is slow but safe */
    for(i = 0; i < len; i++) {
        superstringSet(s2, i, s[i]);
    }

    return s2;
}

/* internal helper for superstringFromFile */
/* resizes a superstring, extending with nulls or truncating
 * as appropriate.*/
/* deallocates and returns 0 on error */
static Superstring *
superstringResize(Superstring *s, size_t new_size)
{
    s->buffer = realloc(s->buffer, new_size+1);
    s->upcased_buffer = realloc(s->upcased_buffer, new_size+1);

    if(s->buffer == 0 || s->upcased_buffer == 0) {
        superstringDestroy(s);
        return 0;
    }

    /* put in new null terminator */
    s->buffer[new_size] = '\0';
    s->buffer[new_size] = '\0';

    if(new_size > s->size) {
        memset(s->buffer + s->size, 0, new_size - s->size);
        memset(s->upcased_buffer + s->size, 0, new_size - s->size);
    }

    s->size = new_size;

    return s;
}

#define FROMFILE_INITIAL_BUFFER_SIZE (1024)

/* create a new superstring whose contents are read from a file */
/* size will be number of characters read before EOF */
Superstring *
superstringFromFile(FILE *f)
{
    Superstring *s;  /* string under construction */
    size_t i;        /* position of next character */
    int c;           /* character from file */

    s = superstringCreate(FROMFILE_INITIAL_BUFFER_SIZE);

    if(s == 0) return 0;

    i=0;
    while(1) {
        c = getc(f);
        if(i >= superstringSize(s)) {
            s = superstringResize(s, superstringSize(s)*2);
            if(s == 0) return 0;
        }

        superstringSet(s, i, c);
        i++;
        if (c == EOF) break;
    }

    /* truncate to actual length */
    return superstringResize(s, i);
}

/* return the size of a superstring
 * this will equal the argument passed to superstringCreate */
size_t
superstringSize(const Superstring *s)
{
    return s->size;
}

/* set a byte of a superstring; returns 1 if position is in range */
/* if position is out of range, do nothing and return 0 */
int
superstringSet(Superstring *s, size_t position, int value)
{
    /* we don't check < 0 because size_t is unsigned */
    if(position >= s->size) {
        return 0;
    } else {
        s->buffer[position] = value;
        s->upcased_buffer[position] = toupper(value);
        return 1;
    }
}

/* return contents of a superstring as a string */
const char *
superstringAsString(const Superstring *s)
{
    return s->buffer;
}

/* return upper-cased version of superstring
 * e.g. puts(superstringUpcased(superstringFromString("abc")))
 * will print "ABC".
 *
 * The user does *not* need to free the returned string,
 * which should remain usable at least until the next
 * time the superstring is modified.
 */
const char *
superstringUpcased(const Superstring *s)
{
    return s->upcased_buffer;
}
