#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "chunk.h"
#define LEXER_IMPL
#include "lexer.h"

bool compile(const char* source, Chunk* chunk) {
    (void) chunk;
    initLexer(source);
    int line = -1;
    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf(" | ");
        }
        printf("%2d '%.*s'\n", token.kind, token.length, token.start);
        if (token.kind == TOKEN_EOF) break;
    }
    return true;
}