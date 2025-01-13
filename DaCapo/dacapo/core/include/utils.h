#ifndef UTILS_H
#define UTILS_H

#include "dacapo/core/include/core.h"

struct matrix_instance{
    int numRows;
    int numCols;
    float *pData;
};

typedef struct mat_shape{
	uint32_t row;
	uint32_t col;
	uint32_t channel;

} MAT_SHAPE;

typedef struct conv_specs{
	uint8_t number_of_filters;
	uint8_t kernel_size;
	uint8_t strides;
} CONV_SPECS;


#define MAIN_BUFFER_SIZE 2*1024
extern uint8_t MAIN_BUFFER[MAIN_BUFFER_SIZE];


typedef struct matrix_instance arm_matrix_instance_f32;

void fill_with(float num, float *array, int size);
void arm_mat_mult_f32(arm_matrix_instance_f32* matA, arm_matrix_instance_f32* matB, arm_matrix_instance_f32* output_vector);

float find_m0(float m);


#endif
