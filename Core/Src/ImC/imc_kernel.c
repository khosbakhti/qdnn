#include <stdio.h>

#include "ImC/imc_kernel.h"
#include "ImC/imc_api.h"
#include "stm32l5xx.h"
#include "cmsis_os.h"

#if(imcUSE_CHECKPOINT_VOLTAGE_CHECK)
    extern ADC_HandleTypeDef ADC_HANDLER_SBC;
#endif

#if(imcUSE_CHECKPOINT_PASS_COUNTER)
    int checkpointCounter = 0;
    const int checkpoint_pass_count = imcCHECKPOINT_PASS_COUNT;
#endif

void imcInit()
{
    FPU->FPCCR &= ~(FPU_FPCCR_LSPEN_Msk); // turn off lazy stacking
}

int needCheckpointExecution() {
    #if(imcUSE_CHECKPOINT_VOLTAGE_CHECK)
        int capacitor_voltage0 = measure_voltage(ADC_HANDLER_SBC, EPS_CAP_ID_SBC);
        return capacitor_voltage0 < imcCHECKPOINT_VOLTAGE;
        printf("\t\t\t\t\t\t[EPS] CAP VOLT: %d.%03d\r\n", capacitor_voltage0 / 1000, capacitor_voltage0 % 1000);
    #endif

    #if(imcUSE_CHECKPOINT_PASS_COUNTER)
        if (checkpointCounter == checkpoint_pass_count) {
            checkpointCounter = 0;
            return 1;
        } else {
            checkpointCounter++;
            return 0;
        }
    #endif

    #if(!imcUSE_CHECKPOINT_VOLTAGE_CHECK && !imcUSE_CHECKPOINT_PASS_COUNTER)
        return 1;
    #endif
}