#ifndef CROSS_ENTROPY_LOSS_H
#define CROSS_ENTROPY_LOSS_H

#include "dacapo/core/include//core.h"



float cross_entropy_loss_forward(layer_t* previous_layer, int * labels);
void cross_entropy_loss_backward(layer_t* previous_layer, int* labels);

#endif
