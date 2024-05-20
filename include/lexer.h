#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <string.h>
#include "common.h"

typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenKind;

typedef struct{
    TokenKind kind;
    const char* start;
    int length;
    int line;
} Token;

typedef struct {
    const char* start;
    const char* current;
    int line;
} Lexer;

Lexer lex;

#define initLexer(source)     \
    do{                       \
      lex.start = (source);   \
      lex.current = (source); \
      lex.line = 1;           \
    }while(false)

Token scanToken(void);

#ifdef LEXER_IMPL

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isAtEnd() {
    return *lex.current == '\0';
}

static char advance_lex() {
    lex.current++;
    return lex.current[-1];
}

static char peek() {
    return *lex.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return lex.current[1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*lex.current != expected) return false;
    lex.current++;
    return true;
}

static Token makeToken(TokenKind kind) {
    Token token;
    token.kind = kind;
    token.start = lex.start;
    token.length = (int)(lex.current - lex.start);
    token.line = lex.line;
    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.kind = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = lex.line;
    return token;
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance_lex();
                break;
            case '\n':
                lex.line++;
                advance_lex();
                break;
            case '/':
            if (peekNext() == '/') {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()) advance_lex();
            } else {
                return;
            }
            break;
            default:
            return;
        }
    }
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') lex.line++;
        advance_lex();
    }
    if (isAtEnd()) return errorToken("Unterminated string.");
    advance_lex();
    return makeToken(TOKEN_STRING);
}

static Token number() {
    while (isDigit(peek())) advance_lex();
        if (peek() == '.' && isDigit(peekNext())) {
            advance_lex();
            while (isDigit(peek())) advance_lex();
        }
        return makeToken(TOKEN_NUMBER);
}



static TokenKind checkKeyword(int start, int length,
const char* rest, TokenKind type) {
    if (lex.current - lex.start == start + length &&
        memcmp(lex.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenKind identifierType() {
    switch (lex.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (lex.current - lex.start > 1) {
                switch (lex.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (lex.current - lex.start > 1) {
                switch (lex.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance_lex();
    return makeToken(identifierType());
}

Token scanToken(void) {
    skipWhitespace();
    lex.start = lex.current;
    if (isAtEnd()) return makeToken(TOKEN_EOF);
    char c = advance_lex();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();
    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }
    return errorToken("Unexpected character.");
}

#endif

#endif // LEXER_H