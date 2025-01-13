#include "dacapo/layers/include/flatten.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"




void compile_flatten(layer_t* this){
    
    layer_t *prev_layer = this->prev_layer;

    tensor_t *result = &this->result;
    result->height = prev_layer->result.height * prev_layer->result.width * prev_layer->result.channel;
    result->width =  1;
    result->channel = 1;
    result->format = HWC;
    result->size_in_bytes = prev_layer->result.size_in_bytes;
    result->size_in_elements = prev_layer->result.size_in_elements;
    result->data = prev_layer->result.data;

    tensor_t *delta = &this->delta;
    delta->height = result->height;
    delta->width = result->width;
    delta->channel = result->channel;
    delta->format = HWC;
    delta->size_in_bytes = result->size_in_bytes;
    delta->size_in_elements = result->size_in_elements;



}


void forward_flatten(layer_t* this){

//    int input_channel = this->prev_layer->result.channel;
//    int input_width = this->prev_layer->result.width;
//    int input_height = this->prev_layer->result.height;
//
//    int output_size_in_elements = this->result.size_in_elements;
//    int8_t *output_data = this->prev_layer->result.data;

//    int k = 0;
//    for(int i = 0; i < input_height; i++){
//        for(int j = 0; j < input_width; j++){
//            for(int c = 0; c < input_channel; c++){
//                int index_in_row_col = i*input_width + j;
//                int channel_selection = input_height*input_width*c;
//                output_data[k] = this->prev_layer->result.data[index_in_row_col + channel_selection];
//                k++;
//            }
//        }
//    }

    //for(int i = 0; i < input_channel*input_height*input_width; i++) printf("%f ", output_data[i]);
}

void backward_flatten(layer_t* this){
    

}


void Flatten(layer_t* flatten){
    
    static int layer_count = 1;
    
    flatten->name = "Flatten";

    flatten->type = FLATTEN;
    flatten->config = 0;

    flatten->compile = compile_flatten;
    flatten->forward = forward_flatten;
    flatten->backward = backward_flatten;

    flatten->layer_id = layer_count;
    flatten->compiled = False;
    layer_count++;

    flatten->inplace_computation = True;

    flatten->prev_layer = NULL;
    flatten->next_layer = NULL;

    flatten->training_requirement = PREVIOUS_LAYER_OUTPUT;

}
