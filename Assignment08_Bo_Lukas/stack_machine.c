// Author: Boyue, Lukas
#include "base.h"

#define STACK_SIZE  10

typedef enum {
    TAG_OPERATION,
    TAG_VALUE,
    TAG_EMPTY,
} Tag;

typedef struct {
    Tag tag;

    union {
        int value;
        char operation;
    };
} StackElement;

typedef struct {
    StackElement elements[STACK_SIZE];
    int stack_pointer;
} Stack;

//checks if char c belongs to an operation
bool is_operation(char c) {
    return c == '+' || c == '/' || c == '-' || c == '*';
}

//checks if c is a digit
bool is_digit(char c){
    return c >= '0' && c <= '9';
}

StackElement make_StackElement(Tag tag) {
    return (StackElement) {
        .tag = tag,
        .value = 0,
    };
}

StackElement make_Value(int value) {
    StackElement e = make_StackElement(TAG_VALUE);
    e.value = value;
    return e;
}

StackElement make_Operation(char operation) {
    StackElement e = make_StackElement(TAG_OPERATION);
    e.operation = operation;
    return e;
}

bool is_empty(Stack* stack){
    return stack->stack_pointer < 0;
}


void print_stack_element(StackElement e) {
    if (e.tag == TAG_OPERATION) {
        printf("[%c]\n", e.operation);
    } else if (e.tag == TAG_VALUE) {
        printf("[%d]\n", e.value);
    } else {
        printf("[ ]\n");
    }
}

void clear_stack(Stack* stack) {
    for (int i = 0; i < STACK_SIZE; ++i) {
        stack->elements[i] = make_StackElement(TAG_EMPTY);
    }
    stack->stack_pointer = -1;
}

void print_stack(Stack* stack) {
    for (int i = 0; i <= stack->stack_pointer; ++i) {
        print_stack_element(stack->elements[i]);
    }
}

/*
push function can place a element(number or operator) in a array with some kind of order,
which determined by the stack pointer. 
one of inputs is a single element,and another is a struct,which include a array,that can contain the element,and a stack pointer,that is more like the index in the array.
output is nothing.
*/
void push(StackElement element, Stack* stack) {
    if(stack->stack_pointer < (STACK_SIZE - 1)){
        stack->stack_pointer++;
        stack->elements[stack->stack_pointer] = element;
    }
}

/*
pop function can take out the element according to its stack pointer,which is previous stored in the the array,
then make the taken one of the origin array initialized , and make a decrement of the stack pointer(that means move to next stack pointer).
input is a wel;-done stuct stack in push function,which contains a array and stack pointer.output is the element in the array. 
*/
StackElement pop(Stack* stack) {
    StackElement e = stack->elements[stack->stack_pointer ];
    stack->elements[stack->stack_pointer] = make_StackElement(TAG_EMPTY);
    stack->stack_pointer--;
    return e;
}

int calc(int a, int b, char c) {
    switch(c) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        default:
            return a / b;
    }
}

void compute(Stack* stack){

    char c = stack->elements[stack->stack_pointer].operation;
    pop(stack);
    char b = stack->elements[stack->stack_pointer].value;
    pop(stack);
    char a = stack->elements[stack->stack_pointer].value;
    pop(stack);

    push(make_Value(calc(a, b, c)), stack); 
}

int main(void) {
    Stack stack;
    clear_stack(&stack);

/*
    int input_size = 128;
    char input[input_size];

    for(int i = 0; i < input_size; i++){
        input[i] = '\0';
    }
    int input_index = 0;
*/
    //printf("=================================\n");
    int c;
    while((c = getchar()) != 'q'){
        if (is_digit(c)) {
            push(make_Value(c - 48), &stack);
        } else if (is_operation(c)) {
            push(make_Operation(c), &stack);
        } else {
            compute(&stack);
        }
    }
    print_stack(&stack);
    return 0;
}
