#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"
#include "stdio.h"

//#define DEBUG_TRACE_EXECUTION

VM vm;

static void resetStack() {
    vm.sp = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {

}

void push(Value value) {
    *vm.sp = value;
    vm.sp++;
}

Value pop() {
    vm.sp--;
    return *vm.sp;
}

static InterpretResult run() {
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (vm.chunk->constants.items[READ_BYTE()])
    #define BINARY_OP(op) \
        do { \
            double b = pop(); \
            double a = pop(); \
            push(a op b); \
        } while (false)
        for (;;) {
            #ifdef DEBUG_TRACE_EXECUTION
                printf(" ");
                for (Value* slot = vm.stack; slot < vm.sp; slot++) {
                    printf("[ ");
                    printValue(*slot);
                    printf(" ]");
                }
                printf("\n");
                disassembleInstruction(vm.chunk,
                (int)(vm.ip - vm.chunk->items));
            #endif
            uint8_t instruction;
            switch (instruction = READ_BYTE()) {
                case OP_RET: {
                    printValue(pop());
                    printf("\n");
                    return INTERPRET_OK;
                }
                case OP_NEGATE: push(-pop()); break;
                case OP_ADD: BINARY_OP(+); break;
                case OP_SUBTRACT: BINARY_OP(-); break;
                case OP_MULTIPLY: BINARY_OP(*); break;
                case OP_DIVIDE: BINARY_OP(/); break;
                case OP_CONST: {
                    Value constant = READ_CONSTANT();
                    push(constant);
                    break;
                }
            }
        }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}

InterpretResult interpret(const char* source){
    Chunk chunk;
    initChunk(&chunk);
        if (!compile(source, &chunk)) {
            freeChunk(&chunk);
            return INTERPRET_COMPILE_ERROR;
        }
        vm.chunk = &chunk;
        vm.ip = vm.chunk->items;
        InterpretResult result = run();
    freeChunk(&chunk);
    return result;
}