#ifndef AVG_POOLING_H
#define AVG_POOLING_H

#include "dacapo/core/include//core.h"

struct AvgPoolConfig
{
    int window_size;
};

typedef struct AvgPoolConfig avg_pool_config_t;


void AvgPooling(layer_t* avg_pooling, avg_pool_config_t *config);

#endif
