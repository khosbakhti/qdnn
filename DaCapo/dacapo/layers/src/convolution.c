#include "dacapo/layers/include/convolution.h"
#include "arm_math_types.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"




/*
 * For cmsis convolution we need the following
 * arm_status arm_convolve_s8(
                           const cmsis_nn_dims *bias_dims,
                           const cmsis_nn_dims *output_dims,
 */




void compile_conv(layer_t* this){

    conv_config_t *config = (conv_config_t*)this->config;
    layer_t *prev_layer = this->prev_layer;


    tensor_t *weight = &this->weight;
    weight->height = config->kernel_size;
    weight->width = config->kernel_size;
    weight->channel = prev_layer->result.channel;
    weight->format = HWC;
    weight->size_in_bytes = weight->height * weight->width * weight->channel * config->num_filters * SIZE_OF_INT8;
    weight->size_in_elements = weight->height * weight->width * weight->channel * config->num_filters;

    tensor_t *bias = &this->bias;
    bias->height = config->num_filters;
    bias->width = 1;
    bias->channel = 1;
    bias->format = HWC;
    bias->size_in_bytes = bias->height * bias->width * bias->channel * SIZE_OF_INT32;
    bias->size_in_elements = bias->height * bias->width * bias->channel;


    tensor_t *result = &this->result;
    result->height = ((prev_layer->result.height - config->kernel_size + 2 * config->padding) / (float)config->stride + 1.0);
    result->width = ((prev_layer->result.width - config->kernel_size + 2 * config->padding) / (float)config->stride + 1.0);
    result->channel = config->num_filters;
    result->format = HWC;
    result->size_in_bytes = result->height * result->width * result->channel * SIZE_OF_INT8;
    result->size_in_elements = result->height * result->width * result->channel;



    this->ctx.buf = MAIN_BUFFER;
    this->ctx.size = MAIN_BUFFER_SIZE;

    this->conv_params.activation.max = 127;
    this->conv_params.activation.min = -128;


//    this->conv_params.input_offset = 128;
//    this->conv_params.output_offset = -128;

    this->conv_params.input_offset = this->qp->input_offset;
    this->conv_params.output_offset = this->qp->output_offset;

//    this->conv_params.input_offset = 0;
//    this->conv_params.output_offset = 0;


    this->conv_params.padding.h = config->padding; this->conv_params.padding.w = config->padding;
    this->conv_params.stride.h = config->stride; this->conv_params.stride.w = config->stride;
    this->conv_params.dilation.h = 1; this->conv_params.dilation.w = 1;

    this->quant_params.multiplier = this->qp->multipliers;
    this->quant_params.shift = this->qp->shifts;




    this->input_dims.c = this->prev_layer->result.channel;;
    this->input_dims.h = this->prev_layer->result.height;
    this->input_dims.n = 1;
    this->input_dims.w = this->prev_layer->result.width;


    this->filter_dims.c = config->num_filters;
    this->filter_dims.h = config->kernel_size;
    this->filter_dims.n = 1;
    this->filter_dims.w = config->kernel_size;

    this->bias_dims.n = 1;
    this->bias_dims.c = config->num_filters;
    this->bias_dims.h = 1;
    this->bias_dims.w = 1;

    this->output_dims.c = this->result.channel;
    this->output_dims.h = this->result.height;
    this->output_dims.w = this->result.width;

    this->output_dims.n = 1;

}


void __conv_forward(arm_matrix_instance_f32 filter, arm_matrix_instance_f32 input, arm_matrix_instance_f32 output, int stride, int padding){
	
}


void forward_conv(layer_t *this){

	int32_t ss = arm_convolve_wrapper_s8_get_buffer_size(&this->conv_params ,&this->input_dims, &this->filter_dims, &this->output_dims);


	this->ctx.size = ss;

	int8_t* result_data = (int8_t*) this->result.data;
	int8_t* buff_data = (int8_t*) this->ctx.buf;

	for(int i = 0; i < this->result.size_in_elements; i++) result_data[i] = 0;
	for(int i = 0; i < ss; i++) buff_data[i] = 0;

	int stats = arm_convolve_wrapper_s8(
			&this->ctx,
			&this->conv_params,
			&this->quant_params,
			&this->input_dims,
			this->prev_layer->result.data,
			&this->filter_dims,
			this->weight.data,
			&this->bias_dims,
			this->bias.data,
			&this->output_dims,
			this->result.data
			);

	if(stats != ARM_MATH_SUCCESS){
		exit(0);
	}
}


void __conv_forward__passing_gradient(arm_matrix_instance_f32 single_filter, arm_matrix_instance_f32 single_channel_gradient, arm_matrix_instance_f32 output_place_for_passing_gradient, int* scratch, int padding, int stride){
	
}


void __conv_forward_for_weight_update(arm_matrix_instance_f32 gradient_of_single_channel, arm_matrix_instance_f32 input, arm_matrix_instance_f32 output_place_for_filter_gradient, int padding, int stride){

}


void backward_conv(layer_t* this){

}


void Conv2d(layer_t* conv, conv_config_t *config, int8_t* weight, int32_t* bias, quant_param_t *qp){

    static int layer_count = 1;
    
    conv->name = "Conv2d";

    conv->type = CONVOLUTIONAL;
    conv->config = config;

    conv->weight.data = weight;
    conv->bias.data = bias;
    
    conv->compile = compile_conv;
    conv->forward = forward_conv;
    conv->backward = backward_conv;

    conv->layer_id = layer_count;
    conv->compiled = False;
    layer_count++;

    conv->inplace_computation = False;

    conv->next_layer = NULL;
    conv->prev_layer = NULL;

    conv->training_requirement = PREVIOUS_LAYER_OUTPUT;
    conv->qp = qp;


}
