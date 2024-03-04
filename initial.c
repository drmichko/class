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
    FILE *dstFile;
    class src = NULL;
    aglGroup ldg = NULL;

    option(argc, argv);

    initboole(optdim);
    initagldim(ffdimen);

    formDegree = optdeg + 1;

    ldg = mkaglGroup(ffdimen);
    pushClass(getboole(), ldg, aglcard(ffdimen), &src);

    if (optwrite)
	dstFile =
	    openClass(optdir, optname, formDegree, optdeg, ffdimen, "w");
    else
	dstFile = stdout;

    pclass(dstFile, src);
    fclose(dstFile);

    return 0;
}
