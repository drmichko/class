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
#include "option.h"

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


int testDistance(boole fct, int s, int goal)
{
    int w, i, j, k, tmp;
    boole aux;
    code cc = rmtranslate(0, s - 1, fct);
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


int main(int argc, char *argv[])
{
    class tmp, list;
    FILE *dstFile = NULL;
    char fn[64];
    int t, num = 0, soluce = 0;
    int score, wt;

    option(argc, argv);

    srandom(time(NULL));
    assert(optdeg <= optdim);
    assert(optval <= optdeg);
    initboole(optdim);
    initagldim(optdim);

    sprintf(fn, "test-%d.out", optjid);
    dstFile = fopen(fn, "w");
    if (!srcFile)
	srcFile = openClass(optdir, optname, optval, optdeg, optdim, "r");

    list = NULL;
    num = 0;
    while ((tmp = loadClass(srcFile))) {
	if (optjobs == 0 || (num % optjobs) == optjid)
	    addClass(tmp, &list);
	num++;
    }

    soluce = 0;
    while (list) {
	tmp = list;
	list = list->next;
	score = weightBoole(tmp->fct);
	t = 0;
	while (t < optround && score >= optgoal) {
	    wt = testDistance(tmp->fct, optval, optgoal);
	    if (wt < score)
		score = wt;
	    t = t + 1;
	}
	if (t == optround) {
	    pclass(dstFile, tmp);
	    fprintf(dstFile, "#score=%d\n", score);
	    soluce++;
	}
	free(tmp);
    }
    printf("\n#soluce=%d\n", soluce);
    fclose(srcFile);
    fclose(dstFile);
    putchar('\n');
    return 0;
}
