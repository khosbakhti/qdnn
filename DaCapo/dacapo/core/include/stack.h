#ifndef STACK_H
#define STACK_H


#include "dacapo/core/include/core.h"



struct Stack
{
    layer_t* data[MAX_LAYERS];
    int head;
    int size;

    void (*push)(struct Stack *this, layer_t *data);
    void (*pop)(struct Stack *this);
    layer_t* (*top)(struct Stack *this);
    int (*is_empty)(struct Stack *this);
    int (*is_full)(struct Stack *this);
};

typedef struct Stack stack_structure_t;

void init_stack(stack_structure_t *s, int size);


#endif
