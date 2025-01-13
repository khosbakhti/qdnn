/*
 * input_data.h
 *
 *  Created on: Sep 9, 2024
 *      Author: MY-PC
 */

#ifndef MODEL_INC_INPUT_DATA_H_
#define MODEL_INC_INPUT_DATA_H_

#define INPUT_SIZE 32*32*3
#include<unistd.h>

extern const int8_t clear_sky_input_image[INPUT_SIZE];
extern const int8_t cloudy_sky_input_image[INPUT_SIZE];

#endif /* MODEL_INC_INPUT_DATA_H_ */
