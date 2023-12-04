#include <stdlib.h>

// Stack structure
struct Stack 
{
    int top;
    unsigned capacity;
    int ***boards;
};

// Stack functions
struct Stack* createStack(unsigned capacity){
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->boards = (int***) malloc(stack->capacity * sizeof(int**));
    return stack;
};


int isFull(struct Stack* stack){
    return stack->top == stack->capacity - 1;
};

int isEmpty(struct Stack* stack){
    return stack->top == -1;
};

void push(struct Stack* stack, int **item){
    if (isFull(stack)){
        stack->capacity *= 2;
        stack->boards = (int***) realloc(stack->boards, stack->capacity * sizeof(int**));
    }
    stack->boards[++stack->top] = item;
};

int** pop(struct Stack* stack){
    if (isEmpty(stack))
        return NULL;
    return stack->boards[stack->top--];
};






