/* Zack Crenshaw
 * CS 152 Win 18
 * HW2 - due 1/22
 */

#include "hw3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char *str_duplicate(char *s)
{
	char *dup = malloc(strlen(s)+1);
	if (dup == NULL) {
		fprintf(stderr, "str_duplicate: malloc failed\n");
		exit (1);
	}
	for (int i = 0; i < strlen(s)+1; i++) {
		dup[i] = s[i];
	}
	return dup;

}

char *cat(char *s1, char *s2)
{
	char *new = malloc(strlen(s1) + strlen(s2) + 1);
	if (new == NULL) {
		fprintf(stderr, "cat: malloc failed\n");
		exit(1);
	}

	for (int i = 0; i<strlen(s1); i++) {
		new[i] = s1[i];
	}

	for (int j = 0; j<strlen(s2) + 1; j++) {
		new[j+strlen(s1)] = s2[j];
	}

	return new;
}


char *catw(char *s1, char sep, char *s2)
{
	char *new = malloc(strlen(s1) + strlen(s2) + 2);
	
	if (new == NULL) {
		fprintf(stderr, "catw: malloc failed\n");
		exit(1);
	}
	for (int i = 0; i<strlen(s1); i++) {
		new[i] = s1[i];
	}
	
	new[strlen(s1)] = sep;

	for (int j = 0; j<strlen(s2); j++) {
		new[j + strlen(s1) + 1] = s2[j];
	}

	new[strlen(s1) + strlen(s2) + 1] = '\0';
	return new;
}	

char *catw_arr(char *ss[], int slen, char sep)
{
	int size = 0;
	int position = 0;
	for (int k = 0; k<slen; k++) {
		size += strlen(ss[k]);
	}
	char *new = malloc(size + slen + 1);
	
	if (new == NULL) {
		fprintf(stderr, "catw_arr: malloc failed\n");
		exit(1);
	
	}
	for (int i = 0; i<slen; i++) {
		for (int j = 0; j<strlen(ss[i]); j++) {
			new[position++] = ss[i][j];
		}
		new[position++] = sep;
	}
	new[size + slen + 1] = '\0';

	return new;
}	

point_t *point_new(double x, double y)
{
	point_t *p =(point_t*)malloc(sizeof(point_t));

	if (p == NULL) {
		fprintf(stderr, "point_new: malloc failed\n");
		exit(1);
	}

	p -> x = x;
	p -> y = y;

	return p;
}

void point_show(FILE *f, point_t *p)
{ 
	fprintf(f,"\n%f , %f",p -> x, p -> y);
}

segment_t *segment_new(point_t *a, point_t *b)
{
	if (a -> x == b -> x && a -> y == b -> y) {
		fprintf(stderr, "segment_new: points are the same\n");
		exit(1);
	} 
	point_t *new_a = point_new(a -> x, a -> y);
        point_t *new_b = point_new(b -> x, b -> y);

	segment_t *new_seg = (segment_t*)malloc(sizeof(segment_t)); 
	if (new_seg == NULL) { 
		fprintf(stderr, "segment_new: malloc failed\n");
		exit(1);
	}
	
	new_seg -> a = new_a;
	new_seg -> b = new_b;

	return new_seg;
}

void segment_show(FILE *f, segment_t *s)
{
	fprintf(f,"%f,%f\n%f,%f\n", s->a->x,s->a->y,s->b->x,s->b->y);
}

void segment_free(segment_t *s)
{
	free(s->a);
	free(s->b);
	free(s);
}

circle_t *circle_new(point_t *center, double r)
{
	if (r<0) {
		fprintf(stderr, "circle_new: radius must be positive\n");
		exit(1);
	}
	point_t *new_c = point_new(center->x,center->y);

	circle_t *circle = malloc(sizeof(circle_t));
	if (circle== NULL) { 
		fprintf(stderr, "circle_new: malloc failed\n");
		exit(1);
	}
	circle -> center = new_c;
	circle -> r = r;
	return circle;
}

void circle_show(FILE *f, circle_t *c)
{
	point_show(f,c -> center);
	fprintf(f," : %f",c -> r);
}

void circle_free(circle_t *circ)
{
	free(circ -> center);
	free(circ);
}

double pt_dist(point_t *a, point_t *b)
{
	return sqrt(pow(a->x - b->x,2.0) + pow(a->y - b->y,2.0));

}

double segment_length(segment_t *s)
{
	return pt_dist(s->a,s->b);
}

double circle_area(circle_t *circ)
{
	return M_PI*pow(circ->r,2.0);
}

int contains(circle_t *circ, point_t *p)
{
	if (pt_dist(circ->center,p) < circ->r) {
		return 1;
	} 
	return 0;
}	

segment_t *horiz_diameter(circle_t *circ)
{
	point_t *c = circ->center;
	double x = c->x;
	double y = c->y;
	double r = circ->r;
	segment_t *d = segment_new(point_new(x-r,y),point_new(x+r,y));
	return d;
}


segment_t *diameter_through(circle_t *circ, point_t *p)
{
	point_t *c = circ->center;
	double cx = c->x; double cy = c->y; double r = circ->r;
	double px = p->x; double py = p->y;

	if (cx == px && cy == py) {
		fprintf(stderr,"diameter_through: point is the center\n");
		exit(1);
	} else {
		if (pt_dist(c,p)<= circ->r) {
			double theta = asin((py - cy) / pt_dist(c,p));
		     	point_t *a = point_new(cx+cos(theta)*r,cy+sin(theta)*r);
			point_t *b = point_new(cx-cos(theta)*r,cy-sin(theta)*r);
			segment_t *ab = segment_new(a,b);
			return ab;
		} else {
			fprintf(stderr,"diameter_through: point not in circle\n");
			exit(1);
		}
	}
}

int overlap(circle_t *circ1, circle_t *circ2)
{
	if (pt_dist(circ1->center,circ2->center) <= circ1->r + circ2->r) {
		return 1;
	}
	return 0;
}

circle_t *circle_translate(circle_t *circ, double dx, double dy)
{
	point_t *c = circ->center;

	circle_t *newc = circle_new(point_new(c->x+dx,c->y+dy),circ->r);
	return newc;
}


