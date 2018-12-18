#include "sal.h"

// HELPER FUNCTIONS

/* tlist: generates test list */
sal *tlist()
{
	sal *list = list_new();
	list->values = malloc(sizeof(char*)*5);
	list->values[0] = strdup("I");
	list->values[1] = strdup("really");
	list->values[2] = strdup("really");
	list->values[3] = strdup("love");
	list->values[4] = strdup("bread");
	list->num_items = 5;
	return list;
}

void char_add(char *s)
{
	while (*s != '\0') {
		(*s)++;
		s++;
	}
}

int ascii_lthan(char *s)
{
	return ((*s) < 106)? 1 : 0;
}

// EVIDENCE FUNCTIONS

/* evidence_list_new: testing list_new */
void evidence_list_new()
{
	printf("*** testing list_new\nexpecting: success\n");
	sal *list = list_new();
	if (list == NULL) {
		printf("failure: list null\n");
	} else if (list->values == NULL && list->num_items == 0) {
		printf("success\n");
	} else {
		printf("failure: other\n");
	}
}

/* evidence_list_show: testing list_show */
void evidence_list_show()
{
	printf("\n*** testing list_show\n");
	sal *list = tlist();
	sal *elist = list_new();
	sal *nllist = NULL;
	printf("expecting: I really really love bread\n");
	list_show(stdout,list,' ');
	printf("expecting: EMPTY LIST\n");
	list_show(stdout,elist,' ');
	printf("expecting: NULL LIST\n");
	list_show(stdout,nllist,' ');
	list_free(elist);
	list_free(list);
}

/* evidence_add_to_front: testing add_to_front */
void evidence_add_to_front()
{
	printf("\n*** testing add_to_front\n");
	sal *list = tlist();
	printf("\nexpecting: He and I really really love bread\n");
	list = add_to_front(list, "He and");
	list_show(stdout,list,' ');
	list_free(list);
}

/* evidence_find_first: testing find first */
void evidence_find_first()
{
	printf("\n*** testing find_first\n");
	sal *list = tlist();
	printf("\nexpecting: 3\n");
	printf("%d",find_first(list,"love"));
	printf("\nexpecting: -1\n");
	printf("%d",find_first(list,"vegetables"));
	printf("\nexpecting: -1\n");
	printf("%d\n",find_first(list_new(),"anything"));
}

/* evidence_get_first: testing get_first */
void evidence_get_first()
{
	printf("\n***testing get_first\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: I\n%s",get_first(list));
	printf("\nexpecting: (null)\n%s",get_first(elist));
	list_free(elist);
	list_free(list);
}

/* evidence_get_last: testing get_last */
void evidence_get_last()
{
	printf("\n***testing get_last\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: bread\n%s",get_last(list));
	printf("\nexpecting: (null)\n%s",get_last(elist));
	list_free(elist);
	list_free(list);
}

/* evidence_get_at_index: testing get_at_index */
void evidence_get_at_index()
{
	printf("\n***testing get_at_index\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: really\n%s",get_at_index(list,1));
	printf("\nexpecting: I\n%s",get_at_index(list, 0));
	printf("\nexpecting: bread\n%s",get_at_index(list,4));
	printf("\nexpecting: (null)\n%s",get_at_index(list,5));
	printf("\nexpecting: (null)\n%s",get_at_index(list, -1));
	printf("\nexpecting: (null)\n%s",get_at_index(elist,0));
}

/* evidence_insert_after: testing insert_after */
void evidence_insert_after()
{
	printf("\n***testing insert_after\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: I don't really really love bread\n");
	list = insert_after(list,"I","don't");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = insert_after(list,"not there","failure");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	elist = insert_after(elist,"not there","failure");
	list_show(stdout,elist,' ');

}

/* evidence_insert_before: testing insert_before */
void evidence_insert_before()
{
	printf("\n***testing insert_before\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: I don't really really love bread\n");
	list = insert_before(list,"really","don't");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = insert_before(list,"not there","failure");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	elist = insert_before(elist,"not there","failure");
	list_show(stdout,elist,' ');
}

/* evidence_insert_at_index: testing insert_at_index */
void evidence_insert_at_index()
{
	printf("\n***testing insert_at_index\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: I don't really really love bread\n");
	list = insert_at_index(list,1,"don't");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = insert_at_index(list,-1,"failure");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	sal *nlist = tlist();
	nlist = insert_at_index(nlist,6,"failure");
	list_show(stdout,nlist,' ');
	printf("\nexpecting: NULL LIST\n");
	elist = insert_at_index(elist,0,"failure");
	list_show(stdout,elist,' ');

}

/* evidence_remove_first: testing remove_first */
void evidence_remove_first()
{
	printf("\n***testing remove_first\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: I really love bread\n");
	list = remove_first(list,"really");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = remove_first(list,"failure");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	elist = remove_first(elist,"failure");
	list_show(stdout,elist,' ');
}

/* evidence_remove_all: testing remove_all */
void evidence_remove_all()
{
	printf("\n***testing remove_all\n");
	sal *list = tlist();
	sal *elist = list_new();
	printf("\nexpecting: I love bread\n");
	list = remove_all(list,"really");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = remove_all(list,"failure");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	elist = remove_all(elist,"failure");
	list_show(stdout,list,' ');
	

}

// evidence_act_on_strings: testing act_on_strings
void evidence_act_on_strings()
{
	sal *list = tlist();
	sal *elist = list_new();
	printf("\n*** testing act_on_strings\n");
	printf("\nexpecting: J sfbmmz sfbmmz mpwf csfbe\n");
	list_show(stdout,act_on_strings(list,char_add),' ');
	list_free(list);
	printf("\nexpecting: EMPTY LIST\n");
	list_show(stdout,act_on_strings(elist, char_add),' ');
	list_free(elist);
}

// evidence_filter: testing filter 
void evidence_filter()
{
	sal *list = tlist();
	sal *elist = list_new();
	printf("\n*** testing filter\n");
	printf("\nexpecting: I bread\n");
	list_show(stdout,filter(list,ascii_lthan),' ');
	printf("\nexpecting: NULL LIST\n");
	list_show(stdout,filter(elist,ascii_lthan),' ');
	list_free(elist);
	list_free(list);
}

/* main: runs all test functions */
int main()
{
	printf("\n--- TESTING HW5 ---\n\n");
	evidence_list_new();
	evidence_list_show();
	evidence_add_to_front();
	evidence_find_first();
	evidence_get_first();
	evidence_get_last();
	evidence_get_at_index();
	evidence_insert_after();
	evidence_insert_before();
	evidence_insert_at_index();
	evidence_remove_first();
	evidence_remove_all();
	evidence_act_on_strings();
	evidence_filter();
	printf("\n\n--- DONE TESTING HW5 ---\n\n");

}
