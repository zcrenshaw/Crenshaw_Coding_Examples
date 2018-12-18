#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hw3.h"

/* evidence_str_duplicate: tests str_duplicate */
void evidence_str_duplicate()
{
	char *test1 = "hello world";
	char *dup1 = str_duplicate(test1);
	printf("\n*** testing str_duplicate\n");
	printf("expecting: hello world\n%s\n",dup1);

}

/* evidence_cat: testing cat */
void evidence_cat()
{
	char *test1 = "hello ";
	char *test2 = "world";
	printf("\n*** testing cat\n");
	printf("expecting: hello world\n%s\n", cat(test1, test2));
}

/* evidence_catw: testing catw */
void evidence_catw()
{
	char *test1 = "hello";
	char sep = ' ';
	char *test2 = "world";
	printf("\n*** testing catw\n");
	printf("expecting: hello world\n%s\n", catw(test1,sep,test2));

}
	
/* evidence_catw_arr: testing catw_arr */
void evidence_catw_arr()
{
	char *test1 = "I";
	char *test2 = "love";
	char *test3 = "bread!";
	char *arr[] = {test1, test2, test3};
	char *new = catw_arr(arr, 3, ' ');

	printf("\n*** testing catw_arr\n");
	printf("expecting: I love bread!\n%s\n",new);

}

/* evidence_point_new: testing point new */
void evidence_point_new()
{
	point_t *point = point_new(1.0,2.5);
	printf("\n*** testing point_new\n");
	printf("expecting 1.0 , 2.5:\n%f , %f\n",point -> x, point ->y);
}

/* evidence_point_show: testing point_show */
void evidence_point_show()
{
	printf("\n*** testing point_show");
	printf("\nexpecting 3.7 , 4.13:");
	point_show(stdout, point_new(3.7,4.13));
}

/* evidence_segment_new: testing segment_new */
void evidence_segment_new()
{
	segment_t *new = segment_new(point_new(1.0,4.1),point_new(2.4,7.8));
	printf("\n\n*** testing segment_new\n");
	printf("expecting:\n1.0,4.1\n2.4,7.8\n");
	printf("%f,%f\n%f,%f\n",new->a->x,new->a->y,new->b->x,new->b->y); 
}

/* evidence_segment_show: testing segment_show */

void evidence_segment_show()
{
	printf("\n*** testing segment_show\n");
	printf("expecting:\n5.3,2.3\n1.8,3.5\n");
	segment_show(stdout,segment_new(point_new(5.3,2.3),point_new(1.8,3.5)));
}

/* evidence_circle_new: testing circle_new */
void evidence_circle_new()
{
	circle_t *test = circle_new(point_new(5.0,8.0),3.0);
	
	printf("\n*** testing circle_new\n");
	printf("expecting: 5.0,8.0 : 3.0\n");
	printf("%f,%f : %f\n",test->center->x, test->center->y, test->r);
}

/* evidence_circle_show: testing circle_show */
void evidence_circle_show()
{
	circle_t *test = circle_new(point_new(7.3,6.2),5.8);
	
	printf("\n*** testing circle_show\n");
	printf("expecting 7.3 , 6.2 : 5.8");
	circle_show(stdout,test);

}


/* evidence_segment_length: testing segment_length */
void evidence_segment_length()
{
	segment_t *ab = segment_new(point_new(0,0),point_new(3,4));
	segment_t *cd = segment_new(point_new(1,1),point_new(-2,-3));
	segment_t *ef = segment_new(point_new(-4,6),point_new(9,-9));

	printf("\n\n***testing segment_length\n");
	printf("expecting 5: %f\n",segment_length(ab));
	printf("expecting 5: %f\n",segment_length(cd));
	printf("expecting 19.85: %f\n",segment_length(ef));


}

/* evidence_circle_area: testing circle area */
void evidence_circle_area()
{
	circle_t *test1 = circle_new(point_new(0,0),1);
	circle_t *test2 = circle_new(point_new(0,0),4.87);
	circle_t *test3 = circle_new(point_new(5,2),3.55);

	printf("\n***testing circle_area\n");
	printf("expecting 3.14: %f\n",circle_area(test1));
	printf("expecting 74.51: %f\n", circle_area(test2));
	printf("expecting 39.59: %f\n", circle_area(test3));

}


/* evidence_contains: testing contains */
void evidence_contains()
{
	circle_t *circ = circle_new(point_new(1,3),sqrt(2));
	point_t *p1 = point_new(1,3);
	point_t *p2 = point_new(2,3);
	point_t *p3 = point_new(0,2);
	point_t *p4 = point_new(-1,0);

	printf("\n*** testing contains\n");
	printf("circle details: ");
	circle_show(stdout,circ);
	printf("\n1,3 expecting true: %d\n",contains(circ,p1));
	printf("2,3 expecting true: %d\n",contains(circ,p2));
	printf("0,2 expecting false: %d\n",contains(circ,p3));
	printf("-1,0 expecting false: %d\n",contains(circ,p4));
	
}

/* evidence_horiz_diameter: testing horiz_diameter */
void evidence_horiz_diameter()
{
	circle_t *circ = circle_new(point_new(1,3),2);
	
	printf("\n*** testing horiz_diameter\n");
	printf("circle details: ");
	circle_show(stdout, circ);
	printf("\nexpecting: -1,3 , 3,3\n");
	segment_show(stdout, horiz_diameter(circ));

}

/* evidence_diameter_through: testing diameter_through */
void evidence_diameter_through()
{
	circle_t *circ = circle_new(point_new(1,3),sqrt(2));
	point_t *p2 = point_new(0,3);
	point_t *p3 = point_new(0,2);
	point_t *p4 = point_new(1.32,3.48);
	point_t *p5 = point_new(1,4);

	printf("\n*** testing diameter_through\n");
	printf("circle details: ");
	circle_show(stdout, circ);
	printf("\n  0,3 expecting: -0.414,3 , 2.414,3\n");
	segment_show(stdout,diameter_through(circ,p2));
	printf("\n  1,4 expecting: 1,4.414 , 1,1.586\n");
	segment_show(stdout,diameter_through(circ,p5));
	printf("\n  0,2 expecting: 0,2 , 2,4\n");
	segment_show(stdout,diameter_through(circ,p3));
	printf("\n  1.32,3.48 expecting: 1.784,4.177 , 0.216,1.823\n");
	segment_show(stdout,diameter_through(circ,p4));

}

/* evidence_overlap: testing overlap */
void evidence_overlap()
{
	circle_t *c1 = circle_new(point_new(1,3),sqrt(2));
	circle_t *c2 = circle_new(point_new(1,2),sqrt(3));
	circle_t *c3 = circle_new(point_new(1,5.41),1);

	printf("\n***testing overlap\n");
	printf("circle 1 details:");
	circle_show(stdout,c1);
	printf("\ncircle 2 details:");
	circle_show(stdout,c2);
	printf("\ncircle 3 details:");
	circle_show(stdout,c3);
	printf("\nc1 and c2: expecting true: %d\n",overlap(c1,c2));
	printf("c1 and c3: expecting true: %d\n",overlap(c1,c3));
	printf("c2 and c3: expecting false: %d\n",overlap(c2,c3));
	printf("c3 and c1: expecting true: %d\n",overlap(c3,c1));
	printf("c3 and c2: expecting false: %d\n",overlap(c3,c2));

}

/* evidence_circle_translate: testing circle_translate */
void evidence_circle_translate()
{
	circle_t *c1 = circle_new(point_new(1,3),sqrt(2));
	
	printf("\n***testing circle_translate\n");
	printf("circle details:");
	circle_show(stdout,c1);
	printf("\n0 , 0  expecting: 1 , 3 : 1.41");
	circle_show(stdout,circle_translate(c1,0,0));
	printf("\n2 , 5  expecting: 3 , 8 : 1.41");
	circle_show(stdout,circle_translate(c1,2,5));
	printf("\n-1 , -3  expecting: 0 , 0 : 1.41");
	circle_show(stdout,circle_translate(c1,-1,-3));
}

/* main: runs all tests */
int main()
{
	printf("\n--- HW3 TESTS ---\n");
	printf("\n -- NOTE: I did not write all the decimal places for expected inputs --\n");
	printf("\n\t Therefore, they are rounded slightly\n");
	evidence_str_duplicate();
	evidence_cat();
	evidence_catw();
	evidence_catw_arr();
	evidence_point_new();
	evidence_point_show();
	evidence_segment_new();
	evidence_segment_show();
	evidence_circle_new();
	evidence_circle_show();
	evidence_segment_length();
	evidence_circle_area();
	evidence_contains();
	evidence_horiz_diameter();
	evidence_diameter_through();
	evidence_overlap();
	evidence_circle_translate();
	printf("\n\n--- END HW3 TESTS ---\n");
	return 0;

}
