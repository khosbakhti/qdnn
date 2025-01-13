#include "dacapo/layers/include/depthwise_convolution.h"


#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"
#include "dacapo/layers/include/convolution.h"


void compile_depth_conv(layer_t *this){

    depth_conv_config_t *config = (depth_conv_config_t*)this->config;
    layer_t *prev_layer = this->prev_layer;


    tensor_t *weight = &this->weight;
    weight->height = config->kernel_size;
    weight->width = config->kernel_size;
    weight->channel = 1;
    weight->format = CHW;
    weight->size_in_bytes = weight->height * weight->width * weight->channel * config->num_filters * SIZE_OF_INT8;
    weight->size_in_elements = weight->height * weight->width * weight->channel * config->num_filters;

    tensor_t *bias = &this->bias;
    bias->height = config->num_filters;
    bias->width = 1;
    bias->channel = 1;
    bias->format = CHW;
    bias->size_in_bytes = bias->height * bias->width * bias->channel * SIZE_OF_INT32;
    bias->size_in_elements = bias->height * bias->width * bias->channel;


    tensor_t *result = &this->result;
    result->height = ((prev_layer->result.height - config->kernel_size + 2 * config->padding) / (float)config->stride + 1.0);
    result->width = ((prev_layer->result.width - config->kernel_size + 2 * config->padding) / (float)config->stride + 1.0);
    result->channel = prev_layer->result.channel;
    result->format = CHW;
    result->size_in_bytes = result->height * result->width * result->channel * SIZE_OF_INT8;
    result->size_in_elements = result->height * result->width * result->channel;



    this->ctx.buf = MAIN_BUFFER;
    this->ctx.size = MAIN_BUFFER_SIZE;

    this->dconv_params.activation.max = 127;
    this->dconv_params.activation.min = -128;

    this->dconv_params.input_offset = this->qp->input_offset;
    this->dconv_params.output_offset = this->next_layer->qp->input_offset;

    this->dconv_params.padding.h = config->padding; this->conv_params.padding.w = config->padding;
    this->dconv_params.stride.h = config->stride; this->conv_params.stride.w = config->stride;
    this->dconv_params.dilation.h = 1; this->conv_params.dilation.w = 1;

    this->quant_params.multiplier = this->qp->multipliers;
    this->quant_params.shift = this->qp->shifts;

    this->input_dims.c = this->prev_layer->result.channel;
    this->input_dims.h = this->prev_layer->result.height;
    this->input_dims.n = 1;
    this->input_dims.w = this->prev_layer->result.width;


    this->filter_dims.c = 1;
    this->filter_dims.h = config->kernel_size;
    this->filter_dims.n = config->num_filters;
    this->filter_dims.w = config->kernel_size;

    this->bias_dims.n = config->num_filters;

    this->output_dims.c = this->result.channel;
    this->output_dims.h = this->result.height;
    this->output_dims.w = this->result.width;
    this->output_dims.n = 1;

}


void forward_depth_conv(layer_t* this){

	int stats = arm_depthwise_conv_wrapper_s8(
			&this->ctx,
			&this->dconv_params,
			&this->quant_params,
			&this->input_dims,
			&this->prev_layer->result.data,
			&this->filter_dims,
			&this->bias_dims,
			&this->output_dims,
			&this->result.data
			);

	if(stats != ARM_MATH_SUCCESS){
		exit(0);
	}

}


void _depth_conv_backward(int kernel_size, int num_filters, int padding, int stride,
                                 float *out_act_map, MAT_SHAPE* out_act_map_shape, 
                                 float *input, MAT_SHAPE* input_shape, 
                                 float *weights, float *bias, 
                                 float *place_for_outgoing_gradient, float *place_for_weight_gradient, float* place_for_bias_gradient){




	

}



void backward_depth_conv(layer_t* this){

    
}








void DepthConv2d(layer_t* depth_conv, depth_conv_config_t *config, int8_t *weight, int8_t *bias, quant_param_t* qp){

    static int layer_count = 1;
    
    depth_conv->name = "DConv2d";

    depth_conv->type = DEPTHCONVOLUTIONAL;
    depth_conv->config = config;

    depth_conv->weight.data = weight;
    depth_conv->bias.data = bias;


    depth_conv->compile = compile_depth_conv;
    depth_conv->forward = forward_depth_conv;
    depth_conv->backward = backward_depth_conv;

    depth_conv->layer_id = layer_count;
    depth_conv->compiled = False;
    layer_count++;

    depth_conv->inplace_computation = False;

    depth_conv->next_layer = NULL;
    depth_conv->prev_layer = NULL;

    depth_conv->training_requirement = PREVIOUS_LAYER_OUTPUT;
    depth_conv->qp = qp;

}
