#include "boolean.h"
#include "classtools.h"
#include "formtools.h"
#include "agltools.h"
#include "orbit.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "time.h"
#include "code.h"

#include "option.h"

boole actboole(boole f, agl s, int val)
{
    boole res = getboole();
    shortvec x, u;

    for (x = 0; x < ffsize; x++)
	res[x] = f[aglImage(x, s)];
    TTtoANF(res);
    for (u = 0; u < ffsize; u++)
	if (weight(u) < val)
	    res[u] = 0;
    ANFtoTT(res);
    return res;
}

int main(int argc, char *argv[])
{
    class tmp, list;
    //FILE  *dstFile = NULL;
    //char  fn[64];
    int num = 0;
    boole f, fs;
    aglGroup gen = NULL, aux = NULL;
    agl s;

    ullong orbsize, tempo;
    mpz_t checksum, cardinal;

    option(argc, argv);

    assert(optdeg <= optdim);
    assert(optval <= optdeg);
    initboole(optdim);
    initagldim(optdim);

    mpz_init(checksum);
    mpz_init(cardinal);
    mpz_set_ui(cardinal, 1);
    mpz_mul_2exp(cardinal, cardinal, rmdimen(optval, optdeg, optdim));

    mpz_set_ui(checksum, 1);
    mpz_mul_2exp(checksum, checksum, rmdimen(optval, optdeg - 1, optdim));

    mpz_sub(cardinal, cardinal, checksum);
    mpz_set_ui(checksum, 0);

    if (!srcFile)
	srcFile = openClass(optdir, optname, optval, optdeg, optdim, "r");


    list = NULL;
    num = 0;
    while ((tmp = loadClass(srcFile))) {
	addClass(tmp, &list);
	num++;
    }
    fclose(srcFile);

    printf("\ndimension B(%d,%d,%d) = %d\n", optval, optdeg, optdim,
	   rmdimen(optval, optdeg, optdim));

    while (list) {
	tmp = list;
	list = list->next;
	if (degree(tmp->fct) != optdeg || valuation(tmp->fct) < optval) {
	    printf("we have a problem panic!");
	    pclass(stdout, tmp);
	    exit(1);
	}
	f = tmp->fct;
	gen = tmp->stab;
	ssinit();
	ssGroup(gen);

	if (tmp->size != ssOrder()) {
	    printf("we have a stab size problem !");
	    pclass(stdout, tmp);
	    exit(1);
	}

	tempo = aglcard(optdim);
	orbsize = tempo / ssOrder();
	mpz_add_ui(checksum, checksum, orbsize);
	ssfree();
	while (gen) {
	    aux = gen;
	    gen = gen->next;
	    s = aux->per;
	    fs = actboole(f, s, optval);
	    if (!boolecmp(f, fs)) {
		printf("we have an other problem panic with the stab !");
		panf(stdout, f);
		pagl(stdout, s);
		exit(1);
	    }
	    free(fs);
	    free(aux);

	}

	free(tmp);
    }

    if (mpz_cmp(cardinal, checksum) != 0) {
	printf("we have a count problem !");
	exit(1);
    }
    mpz_clear(checksum);
    mpz_clear(cardinal);
    printf("\n#%d functions...", num);
    return 0;
}
