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
