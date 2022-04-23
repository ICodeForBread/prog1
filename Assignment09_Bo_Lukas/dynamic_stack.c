// Author: Boyue, Lukas
#include "base.h"

typedef struct dynamic_stack_s {
    int size;
    int value[];
} DynamicStack;

DynamicStack * stack_new() {
    DynamicStack * stack = xcalloc(1, sizeof(DynamicStack));
    return stack;
}

void stack_free(DynamicStack * stack) {  
    free(stack);
}

void stack_push(DynamicStack * stack, int value) {
    stack = realloc(stack, sizeof(DynamicStack) + (stack->size + 1) * sizeof(int));
    stack->value[stack->size] = value;
    stack->size++;
}

int stack_pop(DynamicStack * stack) {
    if (stack->size == 0) {
        return 0;
    }
    stack->size--;
    int output = stack->value[stack->size];
    stack = realloc(stack, sizeof(DynamicStack) + stack->size * sizeof(int));
    return output;
}

void test_stack_ops(DynamicStack * stack) {
    stack_push(stack, 42);
    stack_push(stack, 1);
    stack_push(stack, 13);
    test_equal_i(stack_pop(stack), 13);
    test_equal_i(stack_pop(stack), 1);
    stack_push(stack, 400);
    stack_push(stack, 500);
    stack_push(stack, 600);
    stack_push(stack, 700);
    test_equal_i(stack_pop(stack), 700);
    test_equal_i(stack_pop(stack), 600);
    test_equal_i(stack_pop(stack), 500);
    test_equal_i(stack_pop(stack), 400);
    test_equal_i(stack_pop(stack), 42);
    test_equal_i(stack_pop(stack), 0);
}

int main(void) {
    DynamicStack * stack = stack_new();
    test_stack_ops(stack);

    stack_free(stack);

    DynamicStack * stack2 = stack_new();
    stack_free(stack2);
}
