#ifndef DEPTH_CONVOLUTION_H
#define DEPTH_CONVOLUTION_H

#include "dacapo/core/include//core.h"


struct DepthConvConfig
{
    int kernel_size;
    int stride;
    int padding;
    int num_filters;
};

typedef struct DepthConvConfig depth_conv_config_t;

void DepthConv2d(layer_t* depth_conv, depth_conv_config_t *config, int8_t *weight, int8_t *bias, quant_param_t* qp);

#endif
