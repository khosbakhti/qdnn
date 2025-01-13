
#include "dacapo/core/include//utils.h"





uint8_t MAIN_BUFFER[MAIN_BUFFER_SIZE];


void fill_with(float num, float *array, int size){
    for(int i = 0; i < size; i++) array[i] = num;
}


void arm_mat_mult_f32(arm_matrix_instance_f32* matA, arm_matrix_instance_f32* matB, arm_matrix_instance_f32* output_vector){


	//implement the normal matrix multiplication
	//check if the number of columns of matA is equal to the number of rows of matB
	if(matA->numCols != matB->numRows){
		printf("Matrix multiplication error: number of columns of matA is not equal to the number of rows of matB\n");
		return;
	}

	//implement matrix multiplication
	for(int i = 0; i < matA->numRows; i++){
		for(int j = 0; j < matB->numCols; j++){
			float temp = 0;
			for(int k = 0; k < matA->numCols; k++){
				temp += matA->pData[i * matA->numCols + k] * matB->pData[k * matB->numCols + j];
			}
			output_vector->pData[i * output_vector->numCols + j] = temp;
		}
	}

}
