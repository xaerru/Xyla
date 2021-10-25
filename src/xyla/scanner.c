#include "xyla/scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCAN_SINGLE(c, token)                                                                      \
    case c:                                                                                        \
        return scanner_add_token (token);                                                          \
        break;

typedef struct {
    const char *start;
    const char *current;
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

bool
scanner_match (char expected)
{
    if (scanner_at_end ())
        return false;
    return *scanner.current == expected;
}

char
scanner_peek ()
{
    return *scanner.current;
}

char
scanner_peek_ahead ()
{
    if (scanner_at_end ())
        return '\0';
    return scanner.current[1];
}

char
scanner_advance ()
{
    scanner.current++;
    return scanner.current[-1];
}

Token
scanner_add_error (const char *message)
{
    return (Token){
        .type = TOKEN_ERROR, .start = message, .length = strlen (message), .line = scanner.line
    };
}

void
scanner_skip_whitespace ()
{
    while (1) {
        switch (scanner_peek()) {
            case ' ':
            case '\r':
            case '\t':
                scanner_advance ();
                break;
            case '\n':
                scanner.line++;
                scanner_advance ();
                break;
            case '/':
                if (scanner_peek_ahead () == '/')
                    // Comment
                    while (scanner_peek () != '\n' && !scanner_at_end ())
                        scanner_advance ();
                else
                    return;
                break;
            default:
                return;
        }
    }
}

Token
scanner_add_token (TokenType token)
{
    return (Token){ .type = token,
                    .start = scanner.start,
                    .length = scanner.current - scanner.start,
                    .line = scanner.line };
}

Token
scanner_scan_token ()
{
    scanner_skip_whitespace();
    if (scanner_at_end ())
        scanner_add_token (TOKEN_EOF);
    char c = scanner_advance ();
    switch (c) {
        SCAN_SINGLE ('(', TOKEN_LEFT_PAREN);
        SCAN_SINGLE (')', TOKEN_RIGHT_PAREN);
        SCAN_SINGLE ('{', TOKEN_RIGHT_BRACE);
        SCAN_SINGLE ('}', TOKEN_RIGHT_BRACE);
        SCAN_SINGLE (',', TOKEN_COMMA);
        SCAN_SINGLE ('.', TOKEN_DOT);
        SCAN_SINGLE ('-', TOKEN_MINUS);
        SCAN_SINGLE ('+', TOKEN_PLUS);
        SCAN_SINGLE (';', TOKEN_SEMICOLON);
        SCAN_SINGLE ('*', TOKEN_STAR);
    }
    return scanner_add_error ("Invalid token");
}
