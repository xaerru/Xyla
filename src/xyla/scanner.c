#include "scanner.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREATE_TOKEN(type)                                                                         \
    (Token)                                                                                        \
    {                                                                                              \
        type, scanner.start, (int)(scanner.current - scanner.start), scanner.line                  \
    }
#define CREATE_ERR_TOKEN(message)                                                                  \
    (Token)                                                                                        \
    {                                                                                              \
        TOKEN_ERROR, message, strlen (message), scanner.line                                       \
    }
#define SCAN_SINGLE(c, token)                                                                      \
    case c:                                                                                        \
        return CREATE_TOKEN (token);

typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void
scanner_new (const char *file)
{
    scanner.start = file;
    scanner.current = file;
    scanner.line = 1;
}

static inline bool
scanner_eof ()
{
    return *scanner.current == '\0';
}

static inline bool
scanner_match (char expected)
{
    if (scanner_eof ())
        return false;
    if (*scanner.current != expected)
        return false;

    scanner.current++;
    return true;
}

static inline char
scanner_peek ()
{
    return *scanner.current;
}

static inline char
scanner_peek_ahead ()
{
    if (scanner_eof ())
        return '\0';
    return scanner.current[1];
}

static inline char
scanner_next ()
{
    scanner.current++;
    return scanner.current[-1];
}

static void
scanner_skip_whitespace ()
{
    while (1) {
        switch (scanner_peek ()) {
            case ' ':
            case '\r':
            case '\t':
                scanner_next ();
                break;
            case '\n':
                scanner.line++;
                scanner_next ();
                break;
            case '/':
                if (scanner_peek_ahead () == '/')
                    // Comment
                    while (scanner_peek () != '\n' && !scanner_eof ())
                        scanner_next ();
                else
                    return;
                break;
            default:
                return;
        }
    }
}

static Token
scanner_type_string ()
{
    while ((scanner_peek () != '"' && scanner_peek () != '\'') && !scanner_eof ()) {
        if (scanner_peek () == '\n')
            scanner.line++;
        scanner_next ();
    }
    if (scanner_eof ())
        return CREATE_ERR_TOKEN ("String not terminated");
    scanner_next ();
    return CREATE_TOKEN (TOKEN_STRING);
}

static Token
scanner_type_number ()
{
    while (is_digit ((scanner_peek ())))
        scanner_next ();

    if (scanner_peek () == '.' && is_digit (scanner_peek_ahead ())) {
        scanner_next ();
        while (is_digit (scanner_peek ()))
            scanner_next ();
    }

    return CREATE_TOKEN (TOKEN_NUMBER);
}

static TokenType
scanner_keyword (int start, int length, const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length
        && memcmp (scanner.start + start, rest, length) == 0)
        return type;

    return TOKEN_IDENTIFIER;
}

static TokenType
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

static Token
scanner_type_keyword ()
{
    while (is_alpha (scanner_peek ()) || is_digit (scanner_peek ()))
        scanner_next ();
    return CREATE_TOKEN (scanner_identifier ());
}

Token
scanner_scan_token ()
{
    scanner_skip_whitespace ();
    scanner.start = scanner.current;
    char c = scanner_next ();
    if (scanner_eof ())
        return CREATE_TOKEN (TOKEN_EOF);
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
            return CREATE_TOKEN (scanner_match ('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return CREATE_TOKEN (scanner_match ('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return CREATE_TOKEN (scanner_match ('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return CREATE_TOKEN (scanner_match ('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return scanner_type_string ();
        case '\'':
            return scanner_type_string ();
    }
    return CREATE_ERR_TOKEN ("Invalid token");
}
