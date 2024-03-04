#include "boolean.h"
#include "classtools.h"
#include "formtools.h"
#include "orbit.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "option.h"

int main(int argc, char *argv[])
{
    class tmp, src, dst;
    FILE *dstFile = NULL;
    int num = 0;
    long long  count = 0, total = 0;

    option(argc, argv);

    assert(optdeg <= optdim);

    if (optLevel)
	optval = lastlevel(optdir, optname, optdeg, optdim);

    assert(optval >= 1);

    initboole(optdim);
    initagldim(optdim);


    initForm(optval - 1, optdim);


    if (!srcFile)
	srcFile = openClass(optdir, optname, optval, optdeg, optdim, "r");

    src = NULL;
    num = 0;

    while ((tmp = loadClass(srcFile))) {
	addClass(tmp, &src);
	num++;
    }


    printf("\n#Extension of B(%d, %d, %d)", optval, optdeg, optdim);
    printf("\n#input: %d class...", classNumber(src));
    printf("\n#space of forms of degree %d has dimension %d size %ld...",
	   formDegree, formDimen, formSize);

    dst = NULL;
    uchar *trace;
    while (src) {
	if (optverbe)
	    pclass(stdout, src);
	trace = orbitableClass( src , optround  );
	form v;
	for( v = 0; v < formSize; v++ )
		if ( trace[ v ] == 2 ) count++;
	if (optverbe)
	    printf("count=%d\n", count);
	total+= count;
	popClass(&src);
    	free( trace );
    }

    printf("total=%d\n", total );

    /*
    if (optwrite)
	dstFile =
	    openClass(optdir, optname, formDegree, optdeg, ffdimen, "w");
    else
	dstFile = stdout;

    tmp = dst;
    while (tmp) {
	pclass(dstFile, tmp);
	tmp = tmp->next;
    }
    fclose(dstFile);
   */

    closeForm();
    putchar('\n');
    return 0;
}
