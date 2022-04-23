// Author: Boyue, Lukas
#include "base.h"

char* symbolStr = "()[]{}<>";

int isInStr(char c) {
    char* ptr = symbolStr;
    while (*ptr != '\0') {
        if (c == *ptr) {
            return ptr - symbolStr;
        }
        ptr++;
    }
    return -1;
}

typedef struct node{
    int pos;
    struct node* next;
} Node;

Node* stack = NULL;

void push(int sym) {
    Node* newNode = xmalloc(sizeof(Node));
    newNode->pos = sym;
    newNode->next = stack;
    stack = newNode;
}

void pop(void) {
    Node* toPop = stack;
    stack = stack->next;
    free(toPop);
}

void clearStack(void) {
    while(stack != NULL) {
        pop();
    }
}

bool verify_parentheses(char* text) {
    char* ptr = text;

     while (*ptr != '\0') {
        int pos = isInStr(*ptr);

        if (pos % 2 == 0) {
            push(pos);
        } else if (pos % 2 == 1) {

            if ((pos - 1) == stack->pos) {
                pop();
            } else {
                clearStack();
                return false;
            }

        }
        ptr++;
        
    }

    if (stack == NULL) {
        return true;
    } else {
        clearStack();
        return false;
    }
}

int main(void) {
    report_memory_leaks(true);

    test_equal_i(verify_parentheses(""), true);
    test_equal_i(verify_parentheses("Hello World"), true);
    test_equal_i(verify_parentheses("()"), true);
    test_equal_i(verify_parentheses("<{[()]}>"), true);
    test_equal_i(verify_parentheses("<{[)]}>"), false);
    test_equal_i(verify_parentheses("( Test ) "), true);
    test_equal_i(verify_parentheses("(1+2)*[2+3+(1 + 6/5)]"), true);
    test_equal_i(verify_parentheses("(an unmatched left parenthesis creates an unresolved tension that will stay with you all day"), false);
    test_equal_i(verify_parentheses("< [ > ]"), false);
    test_equal_i(verify_parentheses("<{[()]}"), false);
    test_equal_i(verify_parentheses("(<<({[({<{<({<([[[<[{(<{(<{{<[{<{{{<<{([<<<{{[{<<[[(([{[[[([(({()}))])]]]}]))]]>>}]}}>>>])}>>}}}>}]>}}>)}>)}]>]]])>})>}>})]})>>)"), true);

    return 0;
}
