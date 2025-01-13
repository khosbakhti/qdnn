#ifndef DENSE_H
#define DENSE_H



#include "dacapo/core/include//core.h"


struct DenseConfig
{
    int num_output_nodes;

};

typedef struct DenseConfig dense_config_t;

void Dense(layer_t* dense, dense_config_t *config, int8_t *weight, int32_t *bias, quant_param_t *qp);

#endif
