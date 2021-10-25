#include "xyla/scanner.h"
#include "xyla/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

void
run (char *file)
{
    scanner_new (file);
    scanner_scan_tokens ();
}

int
main (int argc, const char **argv)
{
    if (argc < 1 || argv[1] == NULL) {
        fprintf (stderr, "Usage: xyla [file]\n");
        exit (EX_USAGE);
    }
    char *file = read_file (argv[1]);
    run (file);
}
