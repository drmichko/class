#include "boolean.h"
#include "classtools.h"
#include "formtools.h"
#include "orbit.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
 * ssinit();
                SchreierSims(A, 0);
    res = ssgen();
    ssfree();
    return res;
*/

int main(int argc, char *argv[])
{
    class src = NULL, tmp;
    char *dir = "../data";
    char *name = "class";
    FILE *srcFile;
    int dim = 4, verbe = 0, opt, k = 4, r = 1;
    int write = 0;
    agl U;
    shortvec u;
    uint64_t rop;
    while ((opt = getopt(argc, argv, "d:hk:r:m:n:vw")) != -1) {
	switch (opt) {
	case 'd':
	    dir = strdup(optarg);
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
	case 'w':
	    write = 1;
	    break;
	case 'h':
	default:		/* '?' */
	    fprintf(stderr, "Usage: %s [-hm:v] \n", argv[0]);
	    exit(EXIT_FAILURE);
	}
    }


    assert(k <= dim);
    assert(r <= k);
    initboole(dim);
    initagldim(dim);


    srcFile = openClass(dir, "glclass", dim, k, r, "r");
    while ((tmp = loadClass(srcFile))) {
	tmp->next = src;
	src = tmp;
    }

    fclose(srcFile);
    initForm(r, dim);

    tmp = src;
    U = getaglidt(0);
    while (tmp) {
	rop = tmp->size;
	ssinit();
	ssGroup(tmp->stab);
	for (u = 1; u < ffsize; u++) {
	    U[agldimen] = u;
	    SchreierSims(U, 0);
	}
	aglfreeGroup(tmp->stab);
	tmp->stab = ssgen();
	tmp->size = ssOrder();
	if (tmp->size / rop != ffsize)
	    tmp->size = 0;
	ssfree();
	pclass(stdout, tmp);
	tmp = tmp->next;
    }
    free(U);
    if (write) {
	srcFile = openClass(dir, name, dim, k, r, "w");
	tmp = src;
	while (tmp) {
	    pclass(srcFile, tmp);
	    tmp = tmp->next;
	}
	fclose(srcFile);
    }


    closeForm();
    putchar('\n');
    return 0;
}
