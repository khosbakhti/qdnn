#include "dacapo/layers/include/relu.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"






void compile_relu(layer_t* this){
    
    layer_t *prev_layer = this->prev_layer;

    tensor_t *result = &this->result;
    result->height = prev_layer->result.height;
    result->width =  prev_layer->result.width;
    result->channel = prev_layer->result.channel;
    result->format = CHW;
    result->size_in_bytes = prev_layer->result.size_in_bytes;
    result->size_in_elements = prev_layer->result.size_in_elements;

    tensor_t *delta = &this->delta;
    delta->height = result->height;
    delta->width = result->width;
    delta->channel = result->channel;
    delta->format = CHW;
    delta->size_in_bytes = result->size_in_bytes;


}


void forward_relu(layer_t* this){

    int output_size = this->result.size_in_elements;
    int8_t* output_data = this->result.data;

    int8_t* input_data = this->prev_layer->result.data;

    for(int i = 0; i < output_size; i++){
        if(input_data[i] > 0){
            output_data[i] = input_data[i];
        }
        else{
            output_data[i] = 0;
        }
    }

    layer_t* input_layer = this->prev_layer;    
    int elements_in_channel = input_layer->result.height * input_layer->result.width;

}

void backward_relu(layer_t* this){

}

void Relu(layer_t* relu){
    
    static int layer_count = 1;
    
    relu->name = "Relu";

    relu->type = RELU;
    relu->config = 0;

    relu->compile = compile_relu;
    relu->forward = forward_relu;
    relu->backward = backward_relu;

    relu->layer_id = layer_count;
    relu->compiled = False;
    layer_count++;

    relu->inplace_computation = True;

    relu->next_layer = NULL;
    relu->prev_layer = NULL;

    relu->training_requirement = CURRENT_LAYER_OUTPUT;


}
