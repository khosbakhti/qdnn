#include "ai/simple_model.h"
#include "ai/model_specs.h"
#include "ai/model_weights.h"
#include "ai/quant_params.h"




/*************************************** MODEL 1 ***************************************/


tensor_t input_tensor;
layer_t input_layer;
input_config_t input_config;



layer_t conv_1;
conv_config_t conv_1_config;
quant_param_t conv_1_quant_param;

layer_t flatten;


layer_t dense_1;
dense_config_t dense_config_1;
quant_param_t dense_1_quant_param;

layer_t relu_1;

layer_t dense_2;
dense_config_t dense_config_2;
quant_param_t dense_2_quant_param;




void simple_model(model_t * model, uint8_t* tensor_arena, int memory_size, int8_t* input_image_address){


	init_model(model,tensor_arena, memory_size);

	int input_height = 32;
	int input_width = 32;
	int input_channel = 3;

	//initialize

	input_tensor.data = input_image_address;
	input_tensor.height = input_height;
	input_tensor.width = input_width;
	input_tensor.channel = input_channel;
	input_tensor.format = HWC;
	input_tensor.size_in_bytes = input_tensor.height * input_tensor.width * input_tensor.channel * SIZE_OF_INT8;
	input_tensor.size_in_elements = input_tensor.height * input_tensor.width * input_tensor.channel;
	input_config.input_tensor = input_tensor;
	Input(&input_layer, &input_config);

	conv_1_config.kernel_size = 3;
	conv_1_config.num_filters = 3;
	conv_1_config.stride = 1;
	conv_1_config.padding = 0;

	conv_1_quant_param.input_offset = -m1_conv_1_input_zero_point;
	conv_1_quant_param.output_offset = m1_conv_1_output_zero_point;
	conv_1_quant_param.multipliers = m1_conv_1_multipliers;
	conv_1_quant_param.shifts = m1_conv_1_shifts;
	Conv2d(&conv_1, &conv_1_config, conv_1_weights_original, conv_1_bias_original, &conv_1_quant_param);


	Flatten(&flatten);

	dense_config_1.num_output_nodes = 2;
	dense_1_quant_param.input_offset = m1_dense_1_input_zero_point;
	dense_1_quant_param.multipliers = m1_dense_1_multipliers;
	dense_1_quant_param.shifts = m1_dense_1_shifts;
	dense_1_quant_param.output_offset = m1_dense_1_output_zero_point;

	Dense(&dense_1, &dense_config_1, dense_1_weight_original, dense_1_bias_original, &dense_1_quant_param);


	/***************************************/

	//add the layers to the model in sequence
	model->add(model, &input_layer);
	model->add(model, &conv_1);
	model->add(model, &flatten);
	model->add(model, &dense_1);


	//compile the model
	model->compile(model);

}

/*************************************** MODEL 1 END ***************************************/



/*************************************** MODEL 2 ***************************************/


tensor_t m2_input_tensor;
layer_t m2_input_layer;
input_config_t m2_input_config;



layer_t m2_conv_1;
conv_config_t m2_conv_1_config;
quant_param_t m2_conv_1_quant_param;

layer_t m2_flatten;


layer_t m2_dense_1;
dense_config_t m2_dense_config_1;
quant_param_t m2_dense_1_quant_param;

layer_t m2_relu_1;

layer_t m2_dense_2;
dense_config_t m2_dense_config_2;
quant_param_t m2_dense_2_quant_param;




void simple_model_2(model_t * model, uint8_t* tensor_arena, int memory_size, int8_t* input_image_address){


	init_model(model,tensor_arena, memory_size);

	int input_height = 32;
	int input_width = 32;
	int input_channel = 3;

	//initialize
	m2_input_tensor.data = input_image_address;
	m2_input_tensor.height = input_height;
	m2_input_tensor.width = input_width;
	m2_input_tensor.channel = input_channel;
	m2_input_tensor.format = HWC;
	m2_input_tensor.size_in_bytes = m2_input_tensor.height * m2_input_tensor.width * m2_input_tensor.channel * SIZE_OF_INT8;
	m2_input_tensor.size_in_elements = m2_input_tensor.height * m2_input_tensor.width * m2_input_tensor.channel;
	m2_input_config.input_tensor = m2_input_tensor;
	Input(&m2_input_layer, &m2_input_config);

	m2_conv_1_config.kernel_size = 3;
	m2_conv_1_config.num_filters = 1;
	m2_conv_1_config.stride = 1;
	m2_conv_1_config.padding = 0;

	m2_conv_1_quant_param.input_offset = -m2_conv_1_input_zero_point;
	m2_conv_1_quant_param.output_offset = m2_conv_1_output_zero_point;
	m2_conv_1_quant_param.multipliers = m2_conv_1_multipliers;
	m2_conv_1_quant_param.shifts = m2_conv_1_shifts;
	Conv2d(&m2_conv_1, &m2_conv_1_config, m2_conv_1_weights_original, m2_conv_1_bias_original, &m2_conv_1_quant_param);


	Flatten(&m2_flatten);

	m2_dense_config_1.num_output_nodes = 2;
	m2_dense_1_quant_param.input_offset = m2_dense_1_input_zero_point;
	m2_dense_1_quant_param.multipliers = m2_dense_1_multipliers;
	m2_dense_1_quant_param.shifts = m2_dense_1_shifts;
	m2_dense_1_quant_param.output_offset = m2_dense_1_output_zero_point;

	Dense(&m2_dense_1, &m2_dense_config_1, m2_dense_1_weight_original, m2_dense_1_bias_original, &m2_dense_1_quant_param);


	/***************************************/

	//add the layers to the model in sequence
	model->add(model, &m2_input_layer);
	model->add(model, &m2_conv_1);
	model->add(model, &m2_flatten);
	model->add(model, &m2_dense_1);


	//compile the model
	model->compile(model);

}

/*************************************** MODEL 1 END ***************************************/
