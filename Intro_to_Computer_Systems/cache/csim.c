#include "cachelab.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void helpMenu()
{
	printf("%s\n%s\n","Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>","Options:");
	printf("%s\n%s\n","-h         Print this help message.","-v         Optional verbose flag.");
	printf("%s\n%s\n","-s <num>   Number of set index bits.","-E <num>   Number of lines per set.");
	printf("%s\n%s\n","-b <num>   Number of block offset bits.","-t <file>  Trace file.");
	printf("\n%s\n%s\n","Examples:","linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace");
	printf("%s\n","linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace");
	exit(1);
}

void nullPointerCheck(void* p, char* msg)
{
	if (!p) {
		fprintf(stderr,"%s\n",msg);
		exit(1);
	}
}

void checkCache(int* set_addr, int loop, int* hit, int tag, int iter)
{
	for (int i = 0; i < loop; i+=3) {
		if (*(set_addr + i) == 1) {
			if (*(set_addr + i + 1) == tag) {
				printf(" in cache: %d - tag: %d - addr: %p ",*(set_addr + i + 1),tag,set_addr);
				*hit = 1;
				*(set_addr + i + 2) = iter;
				printf(" hit here: %p  ",set_addr);
				return;
			}
		}
	}
}

void loadNew(int* set_addr, int loop, int* evict, int tag, int iter)
{
	int oldest = *(set_addr + 2);
	int* oldest_addr = set_addr;
	for (int j = 0; j < loop; j+=3) {
		if (*(set_addr + j) == 0) {
//			printf(" cold miss on %lli ",tag);
			*(set_addr + j) = 1;
			*(set_addr + j + 1) = tag;
			*(set_addr + j + 2) = iter;
			printf("  put in here: %p  ",set_addr);
			return;
		} else {
			printf(" in cache: %d - oldest: %d ",*(set_addr + j + 2),oldest);
			if (*(set_addr + j + 2) < oldest) {
				oldest = *(set_addr + j + 2);
				oldest_addr = set_addr + j;
			}
		}
	}
	printf(" evicted addr: %p  ",oldest_addr);
	*(oldest_addr) = 1;
	*(oldest_addr + 1) = tag;
	*(oldest_addr + 2) = iter;
	*evict = 1;		
//	printf(" hot miss on %lli at %p ",tag,oldest_addr);

	return;
}

void simulate(char track[], int iter, int* cache, int s, int e, int b, int addr, char type, int* hc, int* mc, int* ec)
{
	printf("%x\n",addr);
	int s_mask = (1 << s) - 1;
	s_mask <<= b;
	int set = (addr & s_mask) >> b;
	int tag_mask = -1 << (s + b);
	int tag = addr & tag_mask;
//	printf(" tag: %lli - ",tag);
//	printf(" tag: %lli - set: %lli - \n",tag,set);	

	int* set_addr = cache + (set*e*3);
	
	int loop = e*3;
	int hit = 0;
	int evict = 0;

	checkCache(set_addr,loop,&hit,tag,iter);
	if (hit) {
		(*hc)++;
		//printf("hit ");
		track[0] = 'h'; 
		if (type == 'M') {
			(*hc)++;
			//printf("hit");
			track[1] = 'h';
		}
	}
	else {
		(*mc)++;
		//printf("miss ");
		track[0] = 'm';
		loadNew(set_addr,loop,&evict,tag,iter);
		if (evict) {
			(*ec)++;
			//printf("eviction ");
			track[1] = 'e';
		}
		if (type == 'M') {
			/*
			hit = 0;
			evict = 0;
			(*iter)++;
			checkCache(set_addr,loop,&hit,tag,*iter);
			if (hit) 
				(*hc)++;
				track[2] = 'h';
			else {
				(*mc)++;
				loadNew(set_addr,loop,&evict,tag,*iter);
				if (evict)
					(*ec)++;
			}
			*/
			(*hc)++;
			//printf("hit");
			track[2] = 'h';
		}
	}
	return;
}


int main(int argc, char* argv[])
{
	if (argc <= 1) {
		fprintf(stderr,"%s\n","Not enough arguments");
		exit(1);
	}

	if (strcmp(argv[0],"-h") == 0) {
		helpMenu();
	}

	int hc = 0;
	int mc = 0;
	int ec = 0;
	int verbose = 0;
	int s = 0;
	int e = 0;
	int b = 0;
	char* trace = 0;

	if (strcmp(argv[1],"-v") == 0) 
		verbose = 1;
//set variables
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i],"-s") == 0)
			s = atoi(argv[++i]);
		if (strcmp(argv[i],"-E") == 0)
			e = atoi(argv[++i]);
		if (strcmp(argv[i],"-b") == 0)
			b = atoi(argv[++i]);
		if (strcmp(argv[i],"-t") == 0)
			trace = argv[++i];
	}

	if (s <= 0 || e <= 0 || b <= 0 || trace <= 0) {
		fprintf(stderr,"%s\n","Invalid input");
		exit(1);
	}
//set up cache
	int cache_size = 3*(2<<s)*e*(sizeof(int));
	int cache[cache_size];

	for (int c = 0; c < cache_size; c++)
	       cache[c] = 0;	
//begin parsing
	FILE * fp;
	char buf[16];
	int addr;
	char type;
	char* token;
	int iter = 1;
	char report[30];
	char track[3];
	char* m = "miss ";
	char* h = "hit ";
	char* ev = "eviction ";
	fp = fopen(trace, "r");
	nullPointerCheck(fp,"trace file failed to open");

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		iter++;
		if (buf[0] == ' ') {
			char input[30];
			strcpy(input,buf);	
			token = strtok(buf," ,\n");
		       	for (int i = 0; i < 2; i++) {
				if (i == 0)
					type = *token;
				if (i == 1)
					addr = strtol(token,NULL,16);	
				token = strtok(NULL," ,\n");
			}
			simulate(track,iter,cache,s,e,b,addr,type,&hc,&mc,&ec); 	
			if (verbose) {
				for (int t = 0; t < 30; t++)
					report[t] = '\0';
				for (int r = 0; r<3; r++) {
					switch (track[r]) {
						case 'h':
							strcat(report,h);
							break;
						case 'm':
							strcat(report,m);
							break;
						case 'e':
							strcat(report,ev);
						default:
							break;
					}
				}
				printf("%c %x,%s %s\n",type,addr,token,report);
				for (int x = 0; x <3; x++)
					track[e] = '\0';
			}
			printf("\n");

		}	
	}
	
	fclose(fp);	

    printSummary(hc, mc, ec);
    return 0;
}
