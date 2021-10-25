#include "xyla/scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;
const char *source;
Token *tokens;

void
scanner_new (const char *file)
{
    scanner.start = file;
    scanner.current = file;
    scanner.line = 1;
    source = file;
    tokens = malloc (sizeof (Token));
}

bool
scanner_at_end ()
{
    return *scanner.current == '\0';
}

char
scanner_advance ()
{
    scanner.current++;
    return scanner.current[-1];
}

Token
scanner_make_token (TokenType token)
{
    return (Token){ .type = token,
                    .start = scanner.start,
                    .length = scanner.current - scanner.start,
                    .line = scanner.line };
}

Token
scanner_scan_token ()
{
    char c = scanner_advance ();
    switch (c) {
        case '(':
            return scanner_make_token (TOKEN_LEFT_PAREN);
            break;
        case ')':
            return scanner_make_token (TOKEN_RIGHT_PAREN);
            break;
        case '{':
            return scanner_make_token (TOKEN_LEFT_BRACE);
            break;
        case '}':
            return scanner_make_token (TOKEN_RIGHT_BRACE);
            break;
        case ',':
            return scanner_make_token (TOKEN_COMMA);
            break;
        case '.':
            return scanner_make_token (TOKEN_DOT);
            break;
        case '-':
            return scanner_make_token (TOKEN_MINUS);
            break;
        case '+':
            return scanner_make_token (TOKEN_PLUS);
            break;
        case ';':
            return scanner_make_token (TOKEN_SEMICOLON);
            break;
        case '*':
            return scanner_make_token (TOKEN_STAR);
            break;
    }
    return scanner_make_token (TOKEN_EOF);
}
