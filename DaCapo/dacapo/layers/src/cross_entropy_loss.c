#include "dacapo/layers/include/cross_entropy_loss.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"





float cross_entropy_loss_forward(layer_t* previous_layer, int * labels){

//    float sum = 0;
//
//    float *previous_output = previous_layer->result.data;
//
//    int terms = previous_layer->result.size_in_elements;
//
//    for(int i = 0; i < terms; i++){
//        float log_value = log2(previous_output[i]);
//        sum += log_value * labels[i];
//    }
//    return -1 * sum;
}

void cross_entropy_loss_backward(layer_t* previous_layer, int* labels){

//    float *output = previous_layer->delta.data;
//    float *input_matrix = previous_layer->result.data;
//
//    uint32_t terms = previous_layer->result.size_in_elements;
//
//
//	fill_with(0.0, output, terms);
//
//	for(int i = 0; i < terms; i++){
//		if(labels[i] == 1) output[i] = -1.0 * (labels[i]/input_matrix[i]);
//		else output[i] = 0.0;
//	}


}
