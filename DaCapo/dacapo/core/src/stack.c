// This is the stack to store the layers from the same rank simultaneously

#include "dacapo/core/include//stack.h"





void push(struct Stack *this, layer_t *data){
    if(this->is_full(this)){
        return;
    }
    this->head++;
    this->data[this->head] = data;
}

void pop(struct Stack *this){
    if(this->head == -1){
        return;
    }
    this->head--;
}

layer_t* top(struct Stack *this){
    if(this->is_empty(this)){
        return NULL;
    }
    return this->data[this->head];
}

int is_empty(struct Stack *this){
    if(this->head == -1){
        return 1;
    }
    return 0;
}

int is_full(struct Stack *this){
    if(this->head == this->size - 1){
        return 1;
    }
    return 0;
}

void link_stack_functions(stack_structure_t *s, layer_t* (*top)(struct Stack *this), void (*push)(struct Stack *this, layer_t *data), void (*pop)(struct Stack *this), int (*is_empty)(struct Stack *this), int (*is_full)(struct Stack *this)){
    s->push = push;
    s->pop = pop;
    s->top = top;
    s->is_empty = is_empty;
    s->is_full = is_full;
}

void init_stack(stack_structure_t *s, int size){
    s->head = -1;
    s->size = size;
    link_stack_functions(s, top, push, pop, is_empty, is_full);
}
