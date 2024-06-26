#include <stdio.h>
#include "common.h"
#define CHUNK_IMPL
#include "chunk.h"
#include "compiler.h"
#define LEXER_IMPL
#include "lexer.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

Chunk* compilingChunk;

static Chunk* currentChunk() {
    return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);
    if (token->kind == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->kind == TOKEN_ERROR) {

    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char* message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

static void advance_parser() {
    parser.previous = parser.current;
    for (;;) {
        parser.current = scanToken();
        if (parser.current.kind != TOKEN_ERROR) break;
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenKind type, const char* message) {
    if (parser.current.kind == type) {
        advance_parser();
        return;
    }
    errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte);
}

static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }
    return (uint8_t)constant;
}

static void emitConstant(Value value) {
    emitByte(OP_CONST);
    emitByte(makeConstant(value));
}

static void number_expr() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

static void expression() {
    // What goes here?
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

bool compile(const char* source, Chunk* chunk) {
    initLexer(source);
    compilingChunk = chunk;
    parser.hadError = false;
    parser.panicMode = false;
    advance_parser();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    emitByte(OP_RET);
    return !parser.hadError;
}