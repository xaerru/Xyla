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

void
scanner_scan_tokens ()
{
    while (!scanner_at_end ()) {
        scanner.start = scanner.current;
        Token t = scanner_scan_token ();
        printf("%d\n", t.type);
    }
}
