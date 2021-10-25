#include "xyla/scanner.h"
#include "xyla/util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCAN_SINGLE(c, token)                                                                      \
    case c:                                                                                        \
        return scanner_create_token (token);

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
    if (*scanner.current != expected)
        return false;

    scanner.current++;
    return true;
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
scanner_create_token (TokenType token)
{
    return (Token){ .type = token,
                    .start = scanner.start,
                    .length = (int)(scanner.current - scanner.start),
                    .line = scanner.line };
}

Token
scanner_create_error (const char *message)
{
    return (Token){
        .type = TOKEN_ERROR, .start = message, .length = strlen (message), .line = scanner.line
    };
}

void
scanner_skip_whitespace ()
{
    while (1) {
        switch (scanner_peek ()) {
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
scanner_type_string ()
{
    while ((scanner_peek () != '"' && scanner_peek () != '\'') && !scanner_at_end ()) {
        if (scanner_peek () == '\n')
            scanner.line++;
        scanner_advance ();
    }
    if (scanner_at_end ())
        return scanner_create_error ("String not terminated");
    scanner_advance ();
    return scanner_create_token (TOKEN_STRING);
}

Token
scanner_type_number ()
{
    while (is_digit ((scanner_peek ())))
        scanner_advance ();

    if (scanner_peek () == '.' && is_digit (scanner_peek_ahead ())) {
        scanner_advance ();
        while (is_digit (scanner_peek ()))
            scanner_advance ();
    }

    return scanner_create_token (TOKEN_NUMBER);
}

TokenType
scanner_keyword (int start, int length, const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length
        && memcmp (scanner.start + start, rest, length) == 0)
        return type;

    return TOKEN_IDENTIFIER;
}

TokenType
scanner_identifier ()
{
    switch (scanner.start[0]) {
        case 'a':
            return scanner_keyword (1, 2, "nd", TOKEN_AND);
        case 'c':
            return scanner_keyword (1, 4, "lass", TOKEN_CLASS);
        case 'f':
            if (scanner.current - scanner.start > 1)
                switch (scanner.start[1]) {
                    case 'a':
                        return scanner_keyword (2, 3, "lse", TOKEN_FALSE);
                }
            break;
        case 'i':
            return scanner_keyword (1, 1, "n", TOKEN_IN);
        case 'n':
            return scanner_keyword (1, 2, "il", TOKEN_NIL);
        case 'o':
            return scanner_keyword (1, 1, "r", TOKEN_OR);
        case 'r':
            return scanner_keyword (1, 5, "eturn", TOKEN_RETURN);
        case 't':
            if (scanner.current - scanner.start > 1)
                switch (scanner.start[1]) {
                    case 'r':
                        return scanner_keyword (2, 2, "ue", TOKEN_TRUE);
                }
            break;
        case 'l':
            return scanner_keyword (1, 3, "oop", TOKEN_LOOP);
        case 'w':
            return scanner_keyword (1, 3, "hen", TOKEN_WHEN);
    }

    return TOKEN_IDENTIFIER;
}

Token
scanner_type_keyword ()
{
    while (is_alpha (scanner_peek ()) || is_digit (scanner_peek ()))
        scanner_advance ();
    return scanner_create_token (scanner_identifier ());
}

Token
scanner_scan_token ()
{
    scanner_skip_whitespace ();
    scanner.start = scanner.current;
    char c = scanner_advance ();
    if (scanner_at_end ())
        return scanner_create_token (TOKEN_EOF);
    if (is_alpha (c))
        return scanner_type_keyword ();
    if (is_digit (c))
        return scanner_type_number ();
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
        case '!':
            return scanner_create_token (scanner_match ('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return scanner_create_token (scanner_match ('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return scanner_create_token (scanner_match ('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return scanner_create_token (scanner_match ('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return scanner_type_string ();
        case '\'':
            return scanner_type_string ();
    }
    return scanner_create_error ("Invalid token");
}
