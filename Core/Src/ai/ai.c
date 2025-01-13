// #include "ai/dacapo.h"
// #include "ai/input_data.h"
// #include "ai/simple_model.h"
// #include "ImC/imc_api.h"


// extern int8_t input_image[INPUT_SIZE];
// #define ARENA_SIZE 20000
// uint8_t tensor_arena[ARENA_SIZE];

// uint8_t tensor_arena_2[ARENA_SIZE];


// void inference(){

// 	model_t model_1;
// 	simple_model(&model_1, tensor_arena, ARENA_SIZE, input_image);
// 	printf("Model 1 created\r\n");

// 	model_t model_2;
// 	simple_model_2(&model_2, tensor_arena_2, ARENA_SIZE, input_image);
// 	printf("Model 2 created\r\n");

// 	model_1.forward(&model_1);
// 	printf("Model 1 inference completed\r\n");
// 	model_2.forward(&model_2);
// 	printf("Model 2 inference completed\r\n");

//     while(1){
//         //printf("Inference completed\r\n");
//     }

// }