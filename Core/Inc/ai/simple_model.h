#ifndef SIMPLE_MODEL_H
#define SIMPLE_MODEL_H


#include "ai/dacapo.h"






void simple_model(model_t* model,uint8_t *tensor_arena, int memory_size,   int8_t *);
void simple_model_2(model_t * model, uint8_t* tensor_arena, int memory_size, int8_t* input_image_address);


#endif
