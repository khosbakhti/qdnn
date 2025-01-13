#include "dacapo/layers/include/average_pooling.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"






void compile_avg_pooling(layer_t *this){
    
    layer_t *prev_layer = this->prev_layer;
    avg_pool_config_t *config = (avg_pool_config_t*)this->config;

    tensor_t *result = &this->result;
    result->height = prev_layer->result.height / config->window_size;
    result->width =  prev_layer->result.width / config->window_size;
    result->channel = prev_layer->result.channel;
    result->format = CHW;
    result->size_in_bytes = result->height * result->width * result->channel * SIZE_OF_INT8;
    result->size_in_elements = result->height * result->width * result->channel;

}


void forward_avg_pooling(layer_t* this){

    int8_t* output_data = this->result.data;
    int output_height = this->result.height;
    int output_width = this->result.width;
    int output_channel = this->result.channel;
    int output_size_in_bytes = this->result.size_in_bytes;
    int output_size_in_elements = this->result.size_in_elements;

    int8_t* input_data = this->prev_layer->result.data;
    int input_height = this->prev_layer->result.height;
    int input_width = this->prev_layer->result.width;
    int input_channel = this->prev_layer->result.channel;

    avg_pool_config_t *config = (avg_pool_config_t*)this->config;
    int window_size = config->window_size;

    arm_avepool_q7_HWC(input_data,
    		input_height,
			input_channel,
			config->window_size,
			0,
			2,
			output_height,
			MAIN_BUFFER,
			output_data
    		);
}


void backward_avg_pooling(layer_t* this){

}

void AvgPooling(layer_t* avg_pooling, avg_pool_config_t *config){
    
    static int layer_count = 1;
    
    avg_pooling->name = "avg_pooling";

    avg_pooling->type = AVGPOOL;
    avg_pooling->config = config;


    avg_pooling->compile = compile_avg_pooling;
    avg_pooling->forward = forward_avg_pooling;
    avg_pooling->backward = backward_avg_pooling;

    avg_pooling->layer_id = layer_count;
    avg_pooling->compiled = False;
    layer_count++;

    avg_pooling->inplace_computation = False;

    avg_pooling->next_layer = NULL;
    avg_pooling->prev_layer = NULL;

    avg_pooling->training_requirement = PREVIOUS_LAYER_OUTPUT;


}
