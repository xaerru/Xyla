#include "xyla/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

int
main (int argc, char **argv)
{
    if (argc > 1) {
        fprintf (stderr, "Usage: xyla [file]");
        exit (EX_USAGE);
    }
    char *file = read_file (argv[1]);
    printf ("%s", file);
}
