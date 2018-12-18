#include "sdll.h"

/* bread_list: makes a test list (about bread) */
sdll *bread_list()
{
	sdll *list = list_new();
	node *last = node_new("bread!",NULL,NULL);
	node *middle = node_new("love",last,NULL);
	node *first = node_new("I",middle,NULL);
	middle->prev = first;
	last->prev = middle;
	list->first = first;
	list->last = last;
	return list;
}


/* evidence_list_new: testing list new */
void evidence_list_new()
{
	printf("\n***testing list_new\n");
	sdll *nlist = list_new();
	printf("expecting: success!\n");
	if (nlist->first==NULL && nlist->last==NULL) {
		printf("success!\n");
	} else {
		printf("failure :(\n");
	}
	list_free(nlist);

}

/* evidence_node_show: testing node_show */
void evidence_node_show()
{
	node *nd = node_new("hello",NULL,NULL);
	node *null = NULL;
	printf("\n***testing node_show\n");
	printf("expecting: hello\n");
	node_show(stdout,nd);
	printf("\nexpecting: NULL NODE\n");
	node_show(stdout,null);
	free(nd);
}

/* evidence_list_show: testing list_show */
void evidence_list_show()
{
	sdll *list = bread_list();
	sdll *em = list_new();
	sdll *nlist = NULL;

	printf("\n\n***testing list_show\n");
	printf("expecting: I love bread!\n");
	list_show(stdout,list,' ');
	printf("\nexpecting: EMPTY LIST\n");
	list_show(stdout,em,' ');
	printf("\nexpecting: NULL LIST\n");
	list_show(stdout, nlist, ' ');
	list_free(list);
	list_free(em);


}	

/* evidence_find_first: testing find_first */
void evidence_find_first()
{
	sdll *list = bread_list();

	printf("\n*** testing find_first\n");
	printf("expecting: love\n");
	node_show(stdout,find_first(list,"love"));
	printf("\nexpecting: NULL NODE\n");
	node_show(stdout,find_first(list,"world"));
	list_free(list);
}

/* evidence_get_first: testing get_first */
void evidence_get_first()
{
	sdll *list = bread_list();

	printf("\n***testing get_first");
	printf("\nexpecting: I\n");
	node_show(stdout, get_first(list));

}

/* evidence_get_last: testing get_last */
void evidence_get_last()
{
	sdll *list = bread_list();
	printf("\n*** testing get_last\n");
	printf("\nexpecting: bread!\n");
	node_show(stdout, get_last(list));
	list_free(list);
}

/* evidence_get_next: testing get_next */
void evidence_get_next()
{
	sdll *list = bread_list();

	printf("\n***testing get_next");
	printf("\nexpecting: bread!\n");
	node_show(stdout, get_next(list,list->first->next));
	list_free(list);
}

/* evidence_get_prev: testing get_prev */
void evidence_get_prev()
{
	sdll *list = bread_list();

	printf("\n***testing get_next");
	printf("\nexpecting: I\n");
	node_show(stdout, get_previous(list,list->first->next));
	list_free(list);
}

/* evidence_get_at_index: testing get_at_index */
void evidence_get_at_index()
{
	sdll *list = bread_list();

	printf("\n***testing get_at_index");
	printf("\nexpecting: I\n");
	node_show(stdout, get_at_index(list,0));
	printf("\nexpecting: love\n");
	node_show(stdout, get_at_index(list,1));
	printf("\nexpecting: bread!\n");
	node_show(stdout, get_at_index(list,2));
	printf("\nexpecting: NULL NODE\n");
	node_show(stdout, get_at_index(list,3));

	list_free(list);


}

/* evidence_insert_after: testing insert_after */
void evidence_insert_after()
{
	sdll *list = bread_list();

	printf("\n***testing insert_after\n");
	printf("expecting: I really love bread!\n");
	list = insert_after(list,"I","really");
	list_show(stdout,list,' ');
	printf("\nexpecting: I really love bread! a lot\n");
	list = insert_after(list,"bread!","a lot");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = insert_after(list,"string","other string");
	list_show(stdout,list,' ');

}

/* evidence_insert_before: testing insert_before */
void evidence_insert_before()
{
	sdll *list = bread_list();

	printf("\n\n***testing insert_before\n\n");
	printf("expecting: I really love bread!\n");
	list = insert_before(list,"love","really");
	list_show(stdout,list,' ');
	printf("\nexpecting: He and I really love bread!\n");
	list = insert_before(list,"I","He and");
	list_show(stdout,list,' ');
	printf("\nexpecting: He and I really love good bread!\n");
	list = insert_before(list,"bread!","good");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = insert_before(list, "string" , "failure!");
	list_show(stdout,list,' ');

}

/* evidence_insert_at_index: testing insert_at_index */
void evidence_insert_at_index()
{
	sdll *list = bread_list();
	
	printf("\n***testing insert_at_index\n");
	list = insert_at_index(list,1,"really");
	printf("\nexpecting: I really love bread!\n");
	list_show(stdout,list,' ');
	list = insert_at_index(list,0,"He and");
	printf("\nexpecting: He and I really love bread!\n");
	list_show(stdout,list,' ');
	list = insert_at_index(list,4,"good");
	printf("\nexpecting: He and I really love good bread!\n");
	list_show(stdout,list,' ');
	sdll *nlist = list_new();
	nlist = insert_at_index(nlist,0,"fail");
	printf("\nexpecting: NULL LIST\n");
	list_show(stdout,nlist,' ');
	list = insert_at_index(list,12,"fail");
	printf("\nexpecting: NULL LIST\n");
	list_show(stdout,list,' ');
}	

/* evidence_remove_first: testing remove_first */
void evidence_remove_first()
{
	sdll *list = insert_before(bread_list(),"love","love");
	
	printf("\n***testing remove_first\n");
	printf("\noriginal list:\n");
	list_show(stdout,list,' ');
	printf("\nexpecting: I love love\n");
	list = remove_first(list,"bread!");
	list_show(stdout,list,' ');
	printf("\nexpecting: I love\n");
	list = remove_first(list,"love");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = remove_first(list,"fail");
	list_show(stdout,list,' ');

}

/* evidence_remove_all: testing remove_all */
void evidence_remove_all()
{
	sdll *list = insert_before(bread_list(),"love","love");

	printf("\n***testing remove_all\n");
	printf("\noriginal list:\n");
	list_show(stdout,list,' ');
	printf("\nexpecting: I love love\n");
	list = remove_all(list,"bread!");
	list_show(stdout,list,' ');
	printf("\nexpecting: I\n");
	list = remove_all(list,"love");
	list_show(stdout,list,' ');
	printf("\nexpecting: NULL LIST\n");
	list = remove_all(list,"fail");
	list_show(stdout,list,' ');

}

/* main: runs all tests */

int main()
{
	printf("\n--- TESTING HW4 ---\n");
	evidence_list_new();
	evidence_node_show();
	evidence_list_show();
	evidence_find_first();
	evidence_get_first();
	evidence_get_last();
	evidence_get_next();
	evidence_get_prev();
	evidence_get_at_index();
	evidence_insert_after();
	evidence_insert_before();
	evidence_insert_at_index();
	evidence_remove_first();
	evidence_remove_all();
	printf("\n--- TESTING COMPLETE ---\n\n");
	return 0;
}
