#include "dacapo/layers/include/input.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"





void compile_input(layer_t* this){
    
    tensor_t *input_tensor = &(((input_config_t*)this->config)->input_tensor);


    tensor_t *result = &this->result;
    result->height = input_tensor->height;
    result->width =  input_tensor->width;
    result->channel = input_tensor->channel;
    result->format = input_tensor->format;
    result->size_in_bytes = input_tensor->size_in_bytes;
    result->size_in_elements = input_tensor->size_in_elements;
    result->data = input_tensor->data;

    tensor_t *delta = &this->delta;
    delta->height = result->height;
    delta->width = result->width;
    delta->channel = result->channel;
    delta->format = result->format;
    delta->size_in_bytes = result->size_in_bytes;
    delta->size_in_elements = result->size_in_elements;
    delta->data = NULL;

    printf("%s%d Compiled\r\n", this->name, this->layer_id);

}


void forward_input(layer_t* this){

}

void backward_input(layer_t* this){

}

void Input(layer_t* input, input_config_t *config){
    
    static int layer_count = 1;
    
    input->name = "Input";

    input->type = INPUT;
    input->config = config;

    input->compile = compile_input;
    input->forward = forward_input;
    input->backward = backward_input;

    input->layer_id = layer_count;
    input->compiled = False;
    layer_count++;

    input->inplace_computation = False;

    input->next_layer = NULL;
    input->prev_layer = NULL;

    input->training_requirement = NOTHING;
}
