/*
 * ai.h
 *
 *  Created on: Aug 25, 2022
 *      Author: pgj
 */

#ifndef INC_AI_H_
#define INC_AI_H_

// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l496xx.h"
//#define input_size 32*32*3
//#define state_size 4
//extern float input_image[input_size];
//extern float state[state_size];
//extern char float_data[30];
//
//const float model_energy_req[5] = {0.550483672, 0.220868206, 0.186921805, 0.036269573, 0.02993864};

uint8_t* setup(uint8_t *, int, float *scale, int32_t* zero_point);
void preprocess_input();
int perform_inference();
void print_output();



#ifdef __cplusplus
}
#endif

#endif /* INC_AI_H_ */

