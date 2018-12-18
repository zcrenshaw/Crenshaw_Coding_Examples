#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* data definitions */

struct point {
	double x;
	double y;
};
typedef struct point point_t;

struct segment {
	point_t *a;
	point_t *b;
};
typedef struct segment segment_t;

struct circle {
	point_t *center;
	double r;
};
typedef struct circle circle_t;
  
/* *** some string operations *** */

/* Each of these string operations should allocate a new string
 * on the heap and return a pointer to that string. 
 */

/* Create a new heap-allocated copy of the given string. */
/* Implement from scratch; do not call strdup. */
char* str_duplicate(char *s);

/* Build a new string that consists of s1 followed by s2. */
/* ex: cat("a","bc") --> "abc" */
char *cat(char *s1, char *s2);

/* Build a new string with s1, the separator, then s2. */
/* ex: catw("xy",':',"z") --> "xy:z" */
char *catw(char *s1, char sep, char *s2);

/* Concatenate an array of strings with the separator in between. */
/* ex: catw_arr({"a","bb",c"},3,'|') --> "a|bb|c" */
char *catw_arr(char *ss[], int slen, char sep);

/* *** practice with structs *** */

/* --- point fundamentals --- */

/* Build a new heap-allocated point from two doubles. */
point_t *point_new(double x, double y);

/* Use fprintf to print a representation of the point to f */
/* ex: point_show(stdout,p); */
void point_show(FILE *f, point_t *p);

/* Note: there is no need for a specialized point_free operation. */

/* --- segment fundamentals --- */

/* Build a new segment from two points. 
 * Make a deep copy such that both points are new heap allocations.
 * Ensure the points are distinct.
 */
segment_t *segment_new(point_t *a, point_t *b);

/* Print a representation of the segment to f. */
/* Any reasonable textual representation is OK. */
void segment_show(FILE *f, segment_t *s);

/* Free the segment and the points it points to. */
void segment_free(segment_t *s); /* -- BROKEN MUST FIX  --*/

/* --- circle fundamentals --- */

/* Build a new circle from a center and a radius.
 * Make a deep copy such that the center is a new heap allocation.
 * Ensure the radius is positive.
 */
circle_t *circle_new(point_t *center, double r);

/* Print a representation of the circle to f. */
void circle_show(FILE *f, circle_t *c);

/* Free the circle and its center. */
void circle_free(circle_t *circ);

/* --- operations --- */

/* Compute the length of the segment. */
double segment_length(segment_t *s);

/* Compute the area of the circle using M_PI. */
double circle_area(circle_t *circ);

/* Test whether the point is strictly inside the circle.*/
int contains(circle_t *circ, point_t *p);

/* Return the horizontal diameter of the circle. */
/* The order of the points in the segment does not matter. */
segment_t *horiz_diameter(circle_t *circ);

/* Return the diameter that passes through the given point. */
/* The point must be on the circle boundary or inside it, and */
/* must be distinct from the center. */
segment_t *diameter_through(circle_t *circ, point_t *p);

/* Test whether the circles overlap one another. */
/* They overlap if they have at least one point in common. */
int overlap(circle_t *circ1, circle_t *circ2);

/* Build a new transalted circle. */
circle_t *circle_translate(circle_t *circ, double dx, double dy);
