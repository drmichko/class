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

FILE *srcFile = NULL;
char *optdir = "../data";
char *optname = "class";
int optwrite = 0;
int optdim = 7, optdeg = 4, optval = 3, optverbe = 0;
int optjid = 0, optjobs = 1;
int optgoal = 0, optround = 0;
int optLevel = 0;
void help(void)
{
    puts("\t-d directory (../data)");
    puts("\t-j job {0}");
    puts("\t-J process {1}");
    puts("\t-l automatic level");
    puts("\t-n name (class)");
    puts("\t-f input file");
    puts("\t-g goal");
    puts("\t-s valution  (3)");
    puts("\t-t degree    (4)");
    puts("\t-m dimension (7)");
    puts("\t-w write");
}

void option(int argc, char *argv[])
{
    int opt;
    while ((opt =
	    getopt(argc, argv, "f:d:j:J:hk:lr:m:n:vwg:vis:t:")) != -1) {
	switch (opt) {
	case 'd':
	    optdir = strdup(optarg);
	    break;
	case 'f':
	    srcFile = fopen(optarg, "r");
	    if (!srcFile) {
		perror("file");
		exit(1);
	    }
	    break;
	case 'g':
	    optgoal = atoi(optarg);
	    break;
	case 'j':
	    optjid = atoi(optarg);
	    break;
	case 'l':
	    optLevel = 1;
	    break;
	case 'J':
	    optjobs = atoi(optarg);
	    break;
	case 'n':
	    optname = strdup(optarg);
	    break;
	case 'm':
	    optdim = (atoi(optarg));
	    break;
	case 'k':
	    optdeg = atoi(optarg);
	    break;
	case 'r':
	    optround = atoi(optarg);
	    break;
	case 's':
	    optval = atoi(optarg);
	    break;
	case 't':
	    optdeg = atoi(optarg);
	    break;
	case 'v':
	    optverbe = 1;
	    break;
	case 'w':
	    optwrite = 1;
	    break;
	case 'h':
	default:		/* '?' */
	    fprintf(stderr, "Usage: %s  \n", argv[0]);
	    help();
	    exit(EXIT_FAILURE);
	}
    }
}
