#include "scanner.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

void
run (char *file)
{
    scanner_new (file);
    int line = -1;
    while (1) {
        Token token = scanner_scan_token ();
        if (token.type == TOKEN_EOF)
            break;
        if (token.line != line) {
            printf ("%4d ", token.line);
            line = token.line;
        } else {
            printf ("   | ");
        }
        printf ("%2d '%.*s'\n", token.type, token.length, token.start);
    }
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
