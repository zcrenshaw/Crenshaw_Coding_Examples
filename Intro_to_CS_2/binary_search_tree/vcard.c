#include <stdlib.h>
#include <stdio.h>
#include "adrbook.h"

const char *_vcard_todo_format = "TODO [vcard]: %s\nhalting\n";

/* vcard_new : allocate a new vcard, copy all strings, return new object
 * note: this is a "deep copy" as opposed to a "shallow copy"; the string 
 *   arguments are not to be shared by the newly allocated vcard
 */
vcard* vcard_new(const char* cnet, const char* email, 
                 const char* fname, const char* lname, const char* tel)
{
	vcard *vc = calloc(1,sizeof(vcard));
	if (vc ==  NULL) {
		fprintf(stderr,"vcard_new: calloc failed");
		exit(1);
	}
	vc->cnet = strdup(cnet);
	vc->email = strdup(email);
	vc->fname = strdup(fname);
	vc->lname = strdup(lname);
	vc->tel = strdup(tel);
	return vc;
}

/* vcard_free : free vcard and the strings it points to
 */
void vcard_free(vcard* vc)
{
	free(vc->cnet);
	free(vc->email);
	free(vc->fname);
	free(vc->lname);
	free(vc->tel);
	free(vc);
}

void vcard_show(FILE* f, vcard* vc)
{
	fprintf(f,"%s, %s\n",vc->lname,vc->fname);
	fprintf(f,"%s %s\n",vc->cnet,vc->email);
	fprintf(f,"%s\n",vc->tel);
}
