#ifndef CORE_H
#define CORE_H


#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include <math.h>
#include "arm_nn_types.h"
#include "arm_math_types.h"


#define SIZE_OF_INT8 1
#define SIZE_OF_INT32 4
#define MAX_LAYERS 20


enum Bool
{
    False,
    True
};

typedef enum Bool bool_t;



enum DataFormat
{
    HWC,
    CHW,
};

typedef enum DataFormat data_format_t;


struct Tensor
{
    void* data;
    
    int size_in_bytes;
    int size_in_elements;

    int height;
    int width;
    int channel;

    data_format_t format;

};

typedef struct Tensor tensor_t;


struct QuantizationParam {
	int32_t *multipliers;
	int32_t *shifts;
	int32_t input_offset;
	int32_t output_offset;
};

typedef struct QuantizationParam quant_param_t;




enum LayerType
{
    CONVOLUTIONAL,
    DEPTHCONVOLUTIONAL,
    AVGPOOL,
    DENSE,
    SOFTMAX,
    RELU,
    CROSS_ENTROPY_LOSS,
    FLATTEN,
    INPUT,
    BATCHNORMALIZATION

};

typedef enum LayerType layer_type_t;


enum TrainingRequirement{
    PREVIOUS_LAYER_OUTPUT,
    CURRENT_LAYER_OUTPUT,
    NOTHING
};

typedef enum TrainingRequirement training_requirement_t;

struct Layer
{

    //layer specific information
    char* name;
    layer_type_t type;
    void* config;
    
    
    //tensors in this layer
    tensor_t result;
    tensor_t delta; //incoming gradient

    tensor_t weight;
    tensor_t bias;

    //gradient of the parameters
    tensor_t weight_gradient;
    tensor_t bias_gradient;

    //connection to the incoming and outgoing layers
    struct Layer *prev_layer;
    struct Layer *next_layer;

    bool_t compiled;
    bool_t inplace_computation;
    bool_t output_present_in_memory;

    int output_class;



    //function pointers
    void (*compile)(struct Layer*);
    void (*forward)(struct Layer*);
    void (*backward)(struct Layer*);


    //input or output layer
    bool_t is_input;
    bool_t is_output;

    int layer_id;

    training_requirement_t training_requirement;
    
    quant_param_t *qp;
    int32_t *multipliers, *shifts;
    int32_t input_offset, output_offset;

    cmsis_nn_context ctx;


    cmsis_nn_conv_params conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_per_tensor_quant_params tensor_quant_params;

    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;
    cmsis_nn_dw_conv_params dconv_params;

    cmsis_nn_fc_params fc_params;

    uint8_t final_result;


};

typedef struct Layer layer_t;


#endif

