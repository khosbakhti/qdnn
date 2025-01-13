#include "dacapo/layers/include/dense.h"

#include "dacapo/core/include//core.h"
#include "dacapo/core/include//utils.h"


arm_status arm_fully_connected_s8(const cmsis_nn_context *ctx,
                                  const cmsis_nn_fc_params *fc_params,
                                  const cmsis_nn_per_tensor_quant_params *quant_params,
                                  const cmsis_nn_dims *input_dims,
                                  const q7_t *input_data,
                                  const cmsis_nn_dims *filter_dims,
                                  const q7_t *filter_data,
                                  const cmsis_nn_dims *bias_dims,
                                  const int32_t *bias_data,
                                  const cmsis_nn_dims *output_dims,
                                  q7_t *output_data);


void compile_dense(layer_t* this){

    dense_config_t *config = (dense_config_t*)this->config;
    layer_t *prev_layer = this->prev_layer;
    uint32_t num_input_nodes = prev_layer->result.size_in_elements;




    tensor_t *weight = &this->weight;
    weight->height = 1;
    weight->width = num_input_nodes;
    weight->channel = 1;
    weight->format = CHW;
    weight->size_in_bytes = weight->height * weight->width * weight->channel * SIZE_OF_INT8;
    weight->size_in_elements = weight->height * weight->width * weight->channel;


    tensor_t *bias = &this->bias;
    bias->height = config->num_output_nodes;
    bias->width = 1;
    bias->channel = 1;
    bias->format = CHW;
    bias->size_in_bytes = bias->height * bias->width * bias->channel * SIZE_OF_INT32;
    bias->size_in_elements = bias->height * bias->width * bias->channel;


    tensor_t *result = &this->result;
    result->height = config->num_output_nodes;
    result->width = 1;
    result->channel = 1;
    result->format = CHW;
    result->size_in_bytes = result->height * result->width * result->channel * SIZE_OF_INT8;
    result->size_in_elements = result->height * result->width * result->channel;

    this->ctx.buf = MAIN_BUFFER;
    this->ctx.size = MAIN_BUFFER_SIZE;

    this->fc_params.activation.min = -128;
    this->fc_params.activation.max = 127;


//    this->fc_params.input_offset = 128;
//    this->fc_params.output_offset = -26;

    this->fc_params.input_offset = this->qp->input_offset;
    this->fc_params.output_offset = this->qp->output_offset;
    this->fc_params.filter_offset = 0;

    this->tensor_quant_params.multiplier = this->qp->multipliers[0];
    this->tensor_quant_params.shift = this->qp->shifts[0];


    this->input_dims.n = 1;
    this->input_dims.h = 1;
    this->input_dims.w = 1;
    this->input_dims.c = this->prev_layer->result.size_in_elements;


    this->filter_dims.n = this->prev_layer->result.size_in_elements;
    this->filter_dims.h = 1;
    this->filter_dims.w = 1;
    this->filter_dims.c = config->num_output_nodes;

    this->bias_dims.n = 1;
    this->bias_dims.w = 1;
    this->bias_dims.h = 1;
    this->bias_dims.c = config->num_output_nodes;


    this->output_dims.n = 1;
    this->output_dims.h = 1;
    this->output_dims.w = 1;
    this->output_dims.c = config->num_output_nodes;






}


void forward_dense(layer_t *this){

	int stats = arm_fully_connected_s8(
			&this->ctx,
			&this->fc_params,
			&this->tensor_quant_params,
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

	int clear_sky = *((int8_t*)(this->result.data));
	int cloud_sky = *((int8_t*)(this->result.data) + 1);

	if(clear_sky > cloud_sky) this->final_result = 0;
    else this->final_result = 1;
	
}

void backward_dense(layer_t* this){


}

void Dense(layer_t* dense, dense_config_t *config, int8_t *weight, int32_t *bias, quant_param_t *qp){

    static int layer_count = 1;
    
    dense->name = "Dense";

    dense->type = DENSE;
    dense->config = config;

    dense->weight.data = weight;
    dense->bias.data = bias;
    

    dense->compile = compile_dense;
    dense->forward = forward_dense;
    dense->backward = backward_dense;

    dense->layer_id = layer_count;
    dense->compiled = False;
    layer_count++;

    dense->inplace_computation = False;

    dense->next_layer = NULL;
    dense->prev_layer = NULL;

    dense->training_requirement = PREVIOUS_LAYER_OUTPUT;

    dense->qp = qp;


}
