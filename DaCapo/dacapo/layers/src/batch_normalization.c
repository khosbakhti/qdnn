#include "dacapo/layers/include/batch_normalization.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"






void compile_batch_normalization(layer_t* this){
    
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
    delta->size_in_elements = result->size_in_elements;


}


void forward_batch_normalization(layer_t* this){

}

void backward_batch_normalization(layer_t* this){

    //Not yet implemented

}

void BatchNormalization(layer_t* bn, batch_normalization_config_t* config){
    
    static int layer_count = 1;
    
    bn->name = "BN";

    bn->type = BATCHNORMALIZATION;
    bn->config = config;

    bn->compile = compile_batch_normalization;
    bn->forward = forward_batch_normalization;
    bn->backward = backward_batch_normalization;

    bn->layer_id = layer_count;
    bn->compiled = False;
    layer_count++;

    bn->inplace_computation = False;

    bn->next_layer = NULL;
    bn->prev_layer = NULL;

    bn->training_requirement = PREVIOUS_LAYER_OUTPUT;


}
