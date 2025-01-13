#ifndef CONVOLUTION_H
#define CONVOLUTION_H



#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"


struct ConvConfig
{
    int kernel_size;
    int stride;
    int padding;
    int num_filters;
};

typedef struct ConvConfig conv_config_t;



void Conv2d(layer_t* conv, conv_config_t *config, int8_t *weight, int32_t *bias, quant_param_t *qp);

void __conv_forward_for_weight_update(arm_matrix_instance_f32 gradient_of_single_channel, arm_matrix_instance_f32 input, arm_matrix_instance_f32 output_place_for_filter_gradient, int padding, int stride);
void __conv_forward__passing_gradient(arm_matrix_instance_f32 single_filter, arm_matrix_instance_f32 single_channel_gradient, arm_matrix_instance_f32 output_place_for_passing_gradient, int* scratch, int padding, int stride);

#endif
