
#include "dacapo/layers/include/softmax.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"





void compile_softmax(layer_t* this){
    
    layer_t *prev_layer = this->prev_layer;

    tensor_t *result = &this->result;
    result->height = prev_layer->result.height;
    result->width =  prev_layer->result.width;
    result->channel = prev_layer->result.channel;
    result->format = CHW;
    result->size_in_bytes = prev_layer->result.size_in_bytes;
    result->size_in_elements = prev_layer->result.size_in_elements;


}


void forward_softmax(layer_t* this){

    int output_size_in_bytes = this->result.size_in_bytes;
    int output_size_in_elements = this->result.size_in_elements;

    int8_t* output_data = this->result.data;
    int8_t* input_data = this->prev_layer->result.data;


    arm_softmax_q7(
    		input_data,
			this->prev_layer->result.size_in_elements,
			output_data);



    int8_t max_one = output_data[0];
    int max_index = 0;
    for(int i = 1; i < output_size_in_elements; i++){
        printf("\r\n");
        if (output_data[i] > max_one){
            max_one = output_data[i];
            max_index = i;
        }
    }

    this->output_class = max_index;

    //printf("The probabilities are %f and %f \r\n", output_data[0], output_data[1]);
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");




}


void backward_softmax(layer_t* this){


}

void Softmax(layer_t* softmax){
    
    static int layer_count = 1;
    
    softmax->name = "Softmax";

    softmax->type = SOFTMAX;
    softmax->config = 0;

    softmax->compile = compile_softmax;
    softmax->forward = forward_softmax;
    softmax->backward = backward_softmax;

    softmax->layer_id = layer_count;
    softmax->compiled = False;
    layer_count++;

    softmax->inplace_computation = True;

    softmax->next_layer = NULL;
    softmax->prev_layer = NULL;

    softmax->training_requirement = CURRENT_LAYER_OUTPUT;
    

}
