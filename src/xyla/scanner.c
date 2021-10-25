#include "xyla/scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int start;
    int current;
    int line;
} Scanner;

Scanner scanner;
const char* source;

void
scanner_new (const char *file)
{
    scanner.start = 0;
    scanner.current = 0;
    scanner.line = 1;
    source = file;
}

bool
scanner_at_end ()
{
    return scanner.current == strlen (source);
}

char
scanner_advance ()
{
    return source[scanner.current++];
}

Token
scanner_make_token (TokenType token)
{
    return (Token){ .type = token,
                    .start = scanner.start,
                    .length = scanner.current - scanner.start,
                    .line = scanner.line };
}

