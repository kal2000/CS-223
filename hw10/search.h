struct position {
    int x;
    int y;
};

#define NO_PATH (-1)

/* return length of shortest path from source to target 
 * using only points for which blocked returns 0,
 * or NO_PATH if there is no path.
 */
int
search(struct position source, struct position target, int (*blocked)(struct position));
