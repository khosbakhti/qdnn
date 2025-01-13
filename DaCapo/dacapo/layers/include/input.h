#ifndef INPUT_H
#define INPUT_H

#include "dacapo/core/include//core.h"

struct InputConfig
{
    tensor_t input_tensor;    
};


typedef struct InputConfig input_config_t;


void Input(layer_t* input, input_config_t *config);

#endif
