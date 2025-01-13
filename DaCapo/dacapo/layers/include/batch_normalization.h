#ifndef BATCH_NORMALIZATION_H
#define BATCH_NORMALIZATION_H

#include "dacapo/core/include//core.h"

struct BatchNormalizationConfig
{
    float* gamma;
    float* beta;
    float* mean;
    float* variance;
    float epsilon;
};

typedef struct BatchNormalizationConfig batch_normalization_config_t;


void BatchNormalization(layer_t* bn , batch_normalization_config_t* config);

#endif
