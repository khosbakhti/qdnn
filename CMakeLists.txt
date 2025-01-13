cmake_minimum_required(VERSION 3.16...3.24)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# user configurations: use -D option to change these on command line
set(IMC_INSERT_COMPILER_CHECKPOINTS 0 CACHE BOOL "")
set(IMC_USE_CHECKPOINT_PASS_COUNTER 0 CACHE BOOL "")
set(IMC_USE_CHECKPOINT_VOLTAGE_CHECK 1 CACHE BOOL "")
set(IMC_CHECKPOINT_PASS_COUNT 100 CACHE STRING "")

# Host specific settings
set(CMAKE_C_COMPILER /home/khan/Desktop/intermittent/imc_kernel_example_m33/arm_compiler/bin/arm-none-eabi-gcc)
set(CMAKE_SYSROOT "/home/khan/Desktop/intermittent/imc_kernel_example_m33/arm_compiler")
set(ARM_CORTEXM_MULTI_DIR "/home/khan/Desktop/intermittent/imc_kernel_example_m33/arm_compiler/lib/gcc/arm-none-eabi/13.3.1/thumb/v7e-m+fp/hard")
set(ARM_CORTEXM_BUILTINS_DIR "/home/khan/Desktop/intermittent/imc_kernel_example_m33/arm_compiler/lib/gcc/arm-none-eabi/13.3.1/thumb/v8-m.main+fp/hard")
set(OPT_BIN /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/opt)
set(LLC_BIN /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/llc)
set(LLVM_OPT_PLUGIN ${CMAKE_SOURCE_DIR}/lib/libImcInsertCheckpoints.so)

# set(CMAKE_C_COMPILER arm-none-eabi-gcc) #tested with: clang, arm-none-eabi-gcc
# set(CMAKE_C_COMPILER /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/clang)
# set(CMAKE_SYSROOT "/opt/gcc-arm-none-eabi/arm-none-eabi")
# set(ARM_CORTEXM_MULTI_DIR "/opt/gcc-arm-none-eabi/arm-none-eabi/lib/thumb/v8-m.main+fp/hard")
# set(ARM_CORTEXM_BUILTINS_DIR "/opt/gcc-arm-none-eabi/lib/gcc/arm-none-eabi/11.2.1/thumb/v8-m.main+fp/hard")
# set(OPT_BIN /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/opt)
# set(LLC_BIN /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/llc)
# set(LLVM_OPT_PLUGIN ${CMAKE_SOURCE_DIR}/lib/libImcInsertCheckpoints.so)

# set(CMAKE_C_COMPILER /usr/bin/clang)
# set(CMAKE_SYSROOT "/Volumes/MacHD/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi")
# set(ARM_CORTEXM_MULTI_DIR "/Volumes/MacHD/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi/lib/thumb/v8-m.main+fp/hard")
# set(ARM_CORTEXM_BUILTINS_DIR "/Volumes/MacHD/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi/11.2.1/thumb/v8-m.main+fp/hard")
# set(OPT_BIN /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/opt)
# set(LLC_BIN /home/ybkim/workspace/imc/llvm/llvm-17/build/bin/llc)
# set(LLVM_OPT_PLUGIN ${CMAKE_SOURCE_DIR}/lib/libImcInsertCheckpoints.so)

set(SRC_FILES
    USB_Device/Target/usbd_conf.c
    USB_Device/App/usb_device.c
    USB_Device/App/usbd_cdc_if.c
    USB_Device/App/usbd_desc.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    # Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
    # Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    # Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_adc.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_adc_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_cortex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_dma.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_dma_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_exti.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_fdcan.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash_ramfunc.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_gpio.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_gtzc.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_i2c.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_i2c_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_icache.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pcd.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pcd_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_spi.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_spi_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_sram.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_tim.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_tim_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_uart.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_uart_ex.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_ll_fmc.c
    Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_ll_usb.c
    Core/Startup/startup_stm32l552zetx.s
    Core/Src/sc03mpd/sc03mpd.c
    Core/Src/ImC/imc_api.c
    Core/Src/ImC/imc_kernel.c
    DaCapo/dacapo/core/src/core.c
    DaCapo/dacapo/core/src/model.c
    DaCapo/dacapo/core/src/stack.c
    DaCapo/dacapo/core/src/utils.c
    DaCapo/dacapo/layers/src/average_pooling.c
    DaCapo/dacapo/layers/src/batch_normalization.c
    DaCapo/dacapo/layers/src/convolution.c
    DaCapo/dacapo/layers/src/cross_entropy_loss.c
    DaCapo/dacapo/layers/src/dense.c
    DaCapo/dacapo/layers/src/depthwise_convolution.c
    DaCapo/dacapo/layers/src/flatten.c
    DaCapo/dacapo/layers/src/input.c
    DaCapo/dacapo/layers/src/relu.c
    DaCapo/dacapo/layers/src/softmax.c
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_1_x_n_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_wrapper_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c 
    DaCapo/dacapo/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/ActivationFunctions/arm_nn_activations_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/ActivationFunctions/arm_nn_activations_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/ActivationFunctions/arm_relu_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/ActivationFunctions/arm_relu6_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/BasicMathFunctions/arm_elementwise_add_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/BasicMathFunctions/arm_elementwise_mul_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.c
    DaCapo/dacapo/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.c
    DaCapo/dacapo/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.c
    DaCapo/dacapo/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_add_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c 
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mult_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mult_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_nntables.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.c
    DaCapo/dacapo/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c
    DaCapo/dacapo/CMSIS/NN/Source/PoolingFunctions
    DaCapo/dacapo/CMSIS/NN/Source/PoolingFunctions/arm_avgpool_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/PoolingFunctions/arm_max_pool_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c
    DaCapo/dacapo/CMSIS/NN/Source/ReshapeFunctions
    DaCapo/dacapo/CMSIS/NN/Source/ReshapeFunctions/arm_reshape_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/SoftmaxFunctions 
    DaCapo/dacapo/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.c
    DaCapo/dacapo/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.c
    DaCapo/dacapo/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.c
    DaCapo/dacapo/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.c
    DaCapo/dacapo/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.c
    Core/Src/ai/ai.c
    Core/Src/ai/simple_model.c
    Core/Src/ai/input_data.c
    Core/Src/main.c
    Core/Src/stm32l5xx_hal_msp.c
    Core/Src/stm32l5xx_hal_timebase_tim.c
    Core/Src/stm32l5xx_it.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32l5xx.c
)


include_directories(
    DaCapo
    Core/Inc 
    Drivers/STM32L5xx_HAL_Driver/Inc 
    Drivers/STM32L5xx_HAL_Driver/Inc/Legacy 
    Drivers/CMSIS/Device/ST/STM32L5xx/Include 
    DaCapo/dacapo/CMSIS/Core/Include
    DaCapo/dacapo/CMSIS/NN/Include
    DaCapo/dacapo/CMSIS/DSP/Include
    Drivers/CMSIS/Include 
    USB_Device/App 
    USB_Device/Target 
    Middlewares/ST/STM32_USB_Device_Library/Core/Inc 
    Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc 
    Middlewares/Third_Party/FreeRTOS/Source/include 
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure 
)

link_directories(
    lib
    ${ARM_CORTEXM_BUILTINS_DIR}
    ${ARM_CORTEXM_MULTI_DIR}
)

add_compile_definitions(imcUSE_IMC_EXTENSION=0)

if(IMC_USE_CHECKPOINT_PASS_COUNTER)
    if(IMC_USE_CHECKPOINT_VOLTAGE_CHECK)
        message(FATAL_ERROR "IMC_USE_CHECKPOINT_PASS_COUNTER and IMC_USE_CHECKPOINT_VOLTAGE_CHECK cannot be turned on simultaneously")
    endif()
    add_compile_definitions(imcUSE_CHECKPOINT_PASS_COUNTER=1)
    add_compile_definitions(imcUSE_CHECKPOINT_VOLTAGE_CHECK=0)
    add_compile_definitions(imcCHECKPOINT_PASS_COUNT=${IMC_CHECKPOINT_PASS_COUNT})
endif()

if(IMC_USE_CHECKPOINT_VOLTAGE_CHECK)
    if(IMC_USE_CHECKPOINT_PASS_COUNTER)
        message(FATAL_ERROR "IMC_USE_CHECKPOINT_PASS_COUNTER and IMC_USE_CHECKPOINT_VOLTAGE_CHECK cannot be turned on simultaneously")
    endif()
    add_compile_definitions(imcUSE_CHECKPOINT_PASS_COUNTER=0)
    add_compile_definitions(imcUSE_CHECKPOINT_VOLTAGE_CHECK=1)
endif()

# compile flags
add_compile_definitions(USE_HAL_DRIVER STM32L552xx)
set(CMAKE_SYSTEM_NAME Generic)

set(CPU "-mcpu=cortex-m33")
set(FPU "-mfpu=fpv5-sp-d16")
set(FLOAT-ABI "-mfloat-abi=hard")
set(MCU "${CPU} -mthumb ${FPU} ${FLOAT-ABI}")
set(OPT "-O1")
set(C_FLAGS_NO_ASM )
set(C_FLAGS_EXTRA "-Wall -Wno-char-subscripts -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -Wno-format -nostdlib -fdata-sections -ffunction-sections")

set(COMPILER_SPECIFIC_CFLAGS )
if (CMAKE_C_COMPILER MATCHES "clang")
    set(TARGET-TRIPPLE "--target=armv8m-none-eabi --sysroot=${CMAKE_SYSROOT}")
    set(COMPILER_SPECIFIC_CFLAGS "${TARGET-TRIPPLE}")
endif()

set(COMMON_CC_FLAGS "${MCU} ${TARGET-TRIPPLE} ${OPT} ${C_FLAGS_EXTRA} ${COMPILER_SPECIFIC_CFLAGS}")

set(CMAKE_C_FLAGS "${COMMON_CC_FLAGS} ${C_FLAGS_NO_ASM}")

# asm options
set(ASM_OPTIONS "-x assembler-with-cpp -Wno-null-character")
set(CMAKE_ASM_FLAGS "${COMMON_CC_FLAGS} ${ASM_OPTIONS}")

# linker options
set(ARM_CORTEXM_BUILTINS "-L${ARM_CORTEXM_BUILTINS_DIR}")
set(LD_ADDITIONAL_FILES "${ARM_CORTEXM_BUILTINS_DIR}/crti.o ${ARM_CORTEXM_BUILTINS_DIR}/crtn.o ${IMC_LINK_OBJS}")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32L552ZETX_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${LD_ADDITIONAL_FILES} -T ${LINKER_SCRIPT}")

project(imc_example C ASM)
add_executable(imc_example ${SRC_FILES})

if(IMC_APP_FILES)
    if(IMC_INSERT_COMPILER_CHECKPOINTS)
        if(NOT CMAKE_C_COMPILER MATCHES "clang")
            message(FATAL_ERROR "compiler must be clang 17 to enable compiler checkpoint insertion")
        endif()
        add_compile_definitions(imcUSE_IMC_KERNEL=1)
        get_target_property(SRC_INCLUDES imc_example INCLUDE_DIRECTORIES)
        foreach(dir ${SRC_INCLUDES})
            list(APPEND INCLUDE_DIRS "-I${dir}")
        endforeach()
        string(REPLACE " " ";" C_FLAGS ${CMAKE_C_FLAGS})

        set(BASIC_OPT_PASSES "loop-simplify,loop-rotate,mem2reg,instcombine,instsimplify,dce,simplifycfg")
        set(IMC_OPT_PASSES "imc-insert-checkpoints")
        set(IMC_FLAGS ${IMC_BASE})
        set(TARGET_TRIPPLE "--target=armv7em-none-eabi --sysroot=${CMAKE_SYSROOT}")

        foreach(APP_SRC ${IMC_APP_FILES})
            add_custom_command(
                OUTPUT ${PROJECT_BINARY_DIR}/${APP_SRC}.s
                DEPENDS ${APP_SRC}
                # COMMAND ${CLANG_BIN} --target=armv8m-none-eabi --sysroot=${CMAKE_SYSROOT} -g ${C_FLAGS} -O0 -Xclang -disable-O0-optnone ${INCLUDE_DIRS} -D USE_HAL_DRIVER -D STM32L552xx -c -emit-llvm ${PROJECT_SOURCE_DIR}/${APP_SRC} -o ${PROJECT_BINARY_DIR}/${APP_SRC}.bc
                COMMAND ${CMAKE_C_COMPILER} -g ${C_FLAGS} -O0 -Xclang -disable-O0-optnone ${INCLUDE_DIRS} -D USE_HAL_DRIVER -D STM32L552xx -c -emit-llvm ${PROJECT_SOURCE_DIR}/${APP_SRC} -o ${PROJECT_BINARY_DIR}/${APP_SRC}.bc
                COMMAND ${OPT_BIN} -o ${PROJECT_BINARY_DIR}/${APP_SRC}_imc.bc -load-pass-plugin=${LLVM_OPT_PLUGIN} -passes=${BASIC_OPT_PASSES},${IMC_OPT_PASSES} ${PROJECT_BINARY_DIR}/${APP_SRC}.bc
                COMMAND ${OPT_BIN} ${OPT} -o ${PROJECT_BINARY_DIR}/${APP_SRC}.opt.bc ${PROJECT_BINARY_DIR}/${APP_SRC}_imc.bc
                COMMAND ${LLC_BIN} ${IMC_FLAGS} ${CPU} ${OPT} -o ${PROJECT_BINARY_DIR}/${APP_SRC}.s ${PROJECT_BINARY_DIR}/${APP_SRC}.opt.bc
            )
            list(APPEND APP_OPTS ${PROJECT_BINARY_DIR}/${APP_SRC}.s)
        endforeach()
        add_library(imc_apps "${APP_OPTS}")
    else()
        add_library(imc_apps "${IMC_APP_FILES}")
    endif()
endif()

if(IMC_APP_FILES)
    target_link_libraries(imc_example PRIVATE imc_apps imc_freeRTOS c m nosys gcc)
else()
    target_link_libraries(imc_example PRIVATE imc_freeRTOS c m nosys gcc)
endif()

target_compile_options(imc_example PRIVATE "-g")
set_target_properties(imc_example PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})