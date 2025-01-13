#include "dacapo/core/include/model.h"
#include "dacapo/core/include/utils.h"





static void add(model_t *this, layer_t *layer){

    *this->new_layer_place = layer;
    this->new_layer_place++;

    this->usable_memory_space_size -= sizeof(layer_t*);
    this->usable_memory_space_start_address = (uint8_t*)this->new_layer_place;

    if(this->num_added_layers > 0)
        this->link(this->layers[this->num_added_layers-1], this->layers[this->num_added_layers]);

    this->num_added_layers++;

    if(layer->type == SOFTMAX){
    	this->last_layer = layer;
    }

}

static void link(layer_t *first_layer, layer_t *second_layer){

    first_layer->next_layer = second_layer;
    second_layer->prev_layer = first_layer;
 
}

void reset(model_t *this){

}

int max_memory_required_for_inference(model_t *this){

    int max_size= 0;

    for(int i = 0; i < this->num_added_layers; i++){

        layer_t *layer = this->layers[i];

        if(layer->inplace_computation)
            continue;

        int total_size = layer->result.size_in_bytes;

        for(int j = i+1; j < this->num_added_layers; j++){

            layer_t *next_layer = this->layers[j];

            if(next_layer->inplace_computation)
                continue;

            total_size += next_layer->result.size_in_bytes;
            break;

        }

        if(total_size > max_size)
            max_size = total_size;
        
    }

    return max_size;   

}

void _compile_for_inference_only(model_t *this){

    int max_size = max_memory_required_for_inference(this);
    printf("Max size %d\r\n", max_size);

    if(max_size > this->usable_memory_space_size){

        printf("Not enough memory space\r\n");
        exit(1);

    }

    uint8_t *memory_space = this->usable_memory_space_start_address; 

    this->usable_memory_space_size -= max_size;
    this->usable_memory_space_start_address += max_size;
    

    int first_last = 1;
    for(int i = 0; i < this->num_added_layers; i++){

        layer_t *layer = this->layers[i];

        if(layer->type == INPUT)
            continue;

        if(layer->inplace_computation){

            layer->result.data = this->layers[i-1]->result.data;

        }

        else{
                if((first_last % 2) == 1)
                    //allocate memory from the end
                    layer->result.data = (int8_t*)(memory_space + max_size - layer->result.size_in_bytes);

                else
                    //allocate memory from the beginning
                    layer->result.data = (int8_t*)memory_space;

                first_last++;
    
        }
    } 
}


static void compile(model_t *this){

    //the job of this fuction is to initialize the outputs of each layer

    for(int i = 0; i < this->num_added_layers; i++){

        layer_t *layer = this->layers[i];
        // printf("%p\n", layer);

        // printf("Compiling layer %s\n", layer->name);

        layer->compile(layer);
        layer->compiled = True;

    }

    int max_size = 0;
    for(int i = 1; i < this->num_added_layers; i++){

        layer_t *layer = this->layers[i];

        if(layer->result.size_in_bytes > max_size)
            max_size = layer->result.size_in_bytes;

    }

    printf("Maximum Layer Size %d\r\n", max_size);
    
    _compile_for_inference_only(this);

    printf("model Compiled\r\n");
 
}

static void _layer_forward(model_t *this, layer_t* layer){

    layer->forward(layer);
    layer->output_present_in_memory = True;


}
    

static void forward(model_t *this){
    
    
    
    for(int i = 0; i < this->num_added_layers; i++){

        layer_t *layer = this->layers[i];
        this->layer_forward(this, layer);

    }
}

static void add_checkpoint_plan(model_t *this, int *checkpoint_plan){

}



void _layer_backward(model_t *this, layer_t* layer){

    layer->backward(layer);

}

static void backward(model_t *this){

}


static void update(model_t *this,float learning_rate){



}

static int result(model_t *this){
	layer_t *last_one = this->last_layer;

	return last_one->output_class;

}



static void link_model_functions(model_t *this){
    
    this->add = add;
    this->link = link;
    
    this->compile = compile;
    this->add_checkpoint_plan = add_checkpoint_plan;
    
    this->forward = forward;
    this->backward = backward;
    this->layer_forward = _layer_forward;
    this->layer_backward = _layer_backward;

    this->update = update;
    this->reset = reset;


    this->result = result;
}

void init_model(model_t *model, uint8_t *memory_space, int memory_space_size){
    
    model_t* this = model;
    
    this->num_added_layers = 0;
    this->checkpoint_plan = 0;

    this->memory_space_start_address = memory_space;
    this->memory_space_size = memory_space_size;

    this->usable_memory_space_size = memory_space_size;
    this->usable_memory_space_start_address = memory_space;


    this->checkpoint_plan_added = False;
    this->compiled = False;

    this->current_working_rank = 1;

    //in the beginning of the memory space, we store the pointers to the layers
    this->layers = (layer_t**)memory_space;
    this->new_layer_place = (layer_t**)memory_space;

    
    
    link_model_functions(this);

    //init_stack(&this->stack, MAX_LAYERS);



}


