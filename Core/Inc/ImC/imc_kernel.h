#ifndef INC_IMC_IMC_KERNEL_H_
#define INC_IMC_IMC_KERNEL_H_

#ifndef imcUSE_IMC_KERNEL
    #define imcUSE_IMC_KERNEL 1
#endif

#ifndef imcUSE_CHECKPOINT_PASS_COUNTER
    #define imcUSE_CHECKPOINT_PASS_COUNTER 0
#endif

#ifndef imcUSE_CHECKPOINT_VOLTAGE_CHECK
    #define imcUSE_CHECKPOINT_VOLTAGE_CHECK 0
#endif

#ifndef imcCHECKPOINT_PASS_COUNT
    #define imcCHECKPOINT_PASS_COUNT 1000
#endif

#ifndef imcCHECKPOINT_VOLTAGE
    #define imcCHECKPOINT_VOLTAGE 4100 //in mV
#endif

#define imcSTM32_CUBE_IDE 0

#define imcMAX_NUM_TASKS 5
#define imcSTACK_SIZE (4096)

#define IMC_GLOBAL __attribute__((section(".imc_global")))

#define portSVC_IMC_REQUEST_CHECKPOINT 6

#define imcREQUEST_CHECKPOINT() \
    {                           \
        __asm("  svc #6");      \
    }


#include "cmsis_os.h"

void imcInit();
osThreadId_t imcOsThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr);
void vImcInvalidateRecovery();
int needCheckpointExecution();

extern int checkpointCounter;
extern const int checkpoint_pass_count;

#ifndef imcUSE_IMC_EXTENSION
    #define imcUSE_IMC_EXTENSION 0
#endif

#endif
