#ifndef QUANT_SPECS_H
#define QUANT_SPECS_H


int32_t m1_conv_1_multipliers[ 3] = {1714027008, 1630561536, 1496521088};
int32_t m1_conv_1_shifts[ 3] = {-8, -8, -8};
int m1_conv_1_input_zero_point = -128;
int m1_conv_1_output_zero_point = -128;



int32_t m1_dense_1_multipliers[ 2] = {1249636736, 1249636736};
int32_t m1_dense_1_shifts[ 2] = {-11, -11};
int m1_dense_1_input_zero_point = 128;
int m1_dense_1_output_zero_point = -26;




int32_t m2_conv_1_multipliers[ 1] = {1439979264};
int32_t m2_conv_1_shifts[ 1] = {-5};
int m2_conv_1_input_zero_point = -128;
int m2_conv_1_output_zero_point = -128;



int32_t m2_dense_1_multipliers[ 2] = {1801410688, 1801410688};
int32_t m2_dense_1_shifts[ 2] = {-8, -8};
int m2_dense_1_input_zero_point = 128;
int m2_dense_1_output_zero_point = -9;












#endif
