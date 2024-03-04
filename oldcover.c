#include "boolean.h"
#include "classtools.h"
#include "formtools.h"
#include "orbit.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "time.h"
#include "code.h"

/*
boole getcomplement( boole f )
{ boole res = getboole();
  shortvec u;
 xform( f, ffsize  ); 
  for( u = 0; u < ffsize; u++ )
	  if ( f[u] ) res[ u ^ (ffsize-1) ] = 1;
 xform( f, ffsize ); 
 xform( res,  ffsize ); 
 return res;
}

void complement( boole f )
{ boole tmp= getcomplement(f);
  shortvec u;
  for( u = 0; u < ffsize; u++ )
	  f[u] = tmp[u];
  free(tmp);
}
*/


void enumGray(int dimen)
{
    int limite = 1 << dimen;
    unsigned int x = 0, v;
    unsigned int i = 1;
    while (i < limite) {
	v = __builtin_ctz(i);
	x ^= 1 << v;
	i = i + 1;
    }
}


int testDistance(boole fct, int r, int goal)
{
    int w, i, j, k, tmp;
    boole aux;
    code cc = rmtranslate(0, r, fct);
    w = cc.nbl;
    while (w--) {
	i = random() % (cc.nbl - 1);
	j = random() % (cc.nbl - 1);
	aux = cc.fct[i];
	cc.fct[i] = cc.fct[j];
	cc.fct[j] = aux;
	i = random() % ffsize;
	j = random() % ffsize;
	for (k = 0; k < cc.nbl; k++) {
	    tmp = cc.fct[k][i];
	    cc.fct[k][i] = cc.fct[k][j];
	    cc.fct[k][j] = tmp;
	}
    }
    w = reduce(cc);
//pcode(cc);
    freecode(cc);
    return w;
}

int weightBoole(boole f)
{
    int res = 0;
    shortvec x;
    for (x = 0; x < ffsize; x++)
	res += f[x];
    return res;
}

int main(int argc, char *argv[])
{
    class tmp, list, next;
    char *dir = "../data";
    char *name = "class";
    FILE *srcFile = NULL, *dstFile = NULL;
    char fn[64];
    // *dstFile;  
    int dim = 4, opt, k = 4, r = 1, verbe = 0;
    int jid = 0, jobs = 0, num = 0;
    int goal = 0, wt, tour;
    int soluce = 0, old = 0;
    while ((opt = getopt(argc, argv, "f:d:j:J:hk:r:m:n:vwg:vt:")) != -1) {
	switch (opt) {
	case 'd':
	    dir = strdup(optarg);
	    break;
	case 'f':
	    srcFile = fopen(optarg, "r");
	    if (!srcFile) {
		perror("file");
		exit(1);
	    }
	    break;
	case 'g':
	    goal = atoi(optarg);
	    break;
	case 'j':
	    jid = atoi(optarg);
	    break;
	case 'J':
	    jobs = atoi(optarg);
	    break;
	case 'n':
	    name = strdup(optarg);
	    break;
	case 'm':
	    dim = (atoi(optarg));
	    break;
	case 'k':
	    k = atoi(optarg);
	    break;
	case 'r':
	    r = atoi(optarg);
	    break;
	case 'v':
	    verbe = 1;
	    break;
	case 'h':
	default:		/* '?' */
	    fprintf(stderr, "Usage: %s [-hm:v] \n", argv[0]);
	    exit(EXIT_FAILURE);
	}
    }


    srandom(time(NULL));
    assert(k <= dim);
    assert(r <= k);
    initboole(dim);
    initagldim(dim);

    sprintf(fn, "test-%d.txt", jid);
    dstFile = fopen(fn, "w");
    if (!srcFile)
	srcFile = openClass(dir, name, dim, k, r, "r");

    list = NULL;
    num = 0;
    while ((tmp = loadClass(srcFile))) {
	if (jobs == 0 || (num % jobs) == jid)
	    addClass(tmp, &list);
	num++;
    }
    tour = 0;
    while (list) {
	old = soluce;
	soluce = 0;
	next = NULL;
	while (list) {
	    tmp = list;
	    list = list->next;
	    wt = weightBoole(tmp->fct);
	    if (wt >= goal)
		wt = testDistance(tmp->fct, r, goal);
	    if (wt >= goal) {
		if (verbe) {
		    pclass(stdout, tmp);
		    printf("wt=%d\n", wt);
		}
		soluce++;
		addClass(tmp, &next);
	    } else
		free(tmp);
	}
	tour++;
	if (old != soluce || tour == 1000000) {
	    printf("tour=%d soluce=%d\n", tour, soluce);
	    tour = 0;
	}
	list = next;
    }
    printf("\nsoluce=%d\n", soluce);
    fclose(srcFile);
    fclose(dstFile);
    putchar('\n');
    return 0;
}
