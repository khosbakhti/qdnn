#ifndef MODEL_H
#define MODEL_H

#include "dacapo/core/include/core.h"
#include "dacapo/core/include/stack.h"

struct Model {

    //stack for storing the checkpointed layers
    stack_structure_t stack;
    uint8_t* checkpoint_storage_address;
    uint8_t* current_checkpoint_storage_address;
    int checkpoint_storage_size;
    int checkpoint_storage_available_size;
    
    int* checkpoint_plan;

    uint8_t* memory_space_start_address;
    int memory_space_size;

    int gradient_space_size;

    uint8_t* usable_memory_space_start_address;
    int usable_memory_space_size;

    
    bool_t checkpoint_plan_added;
    bool_t compiled;

    //A layer is a node in the graph and the edges are the tensors
    layer_t** layers; 
    layer_t** new_layer_place;
    int num_added_layers;
    int current_working_rank;

    void (*add)(struct Model *this,layer_t* layer);
    void (*link)(layer_t* first_layer, layer_t* second_layer);
    
    //for initializing and compiling the graph
    void (*compile)(struct Model *this);
    void (*add_checkpoint_plan)(struct Model *this,int *checkpoint_plan);
    
    void (*forward)(struct Model *this);
    void (*backward)(struct Model *this);

    void (*layer_forward)(struct Model *this,layer_t*);
    void (*layer_backward)(struct Model *this,layer_t*);

    void (*update)(float learning_rate);
    void (*reset)(struct Model *this);
    void (*summary)(struct Model *this);
    int  (*result)(struct Model *this);

    layer_t* last_layer;



};

typedef struct Model model_t;

void init_model(model_t* model, uint8_t* memory_space, int memory_space_size);


#endif
