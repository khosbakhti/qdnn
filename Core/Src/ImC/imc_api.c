/*
 * imc_api.c
 *
 *  Created on: Jul 21, 2022
 *      Author: yoojin.lim (yoojin.lim@etri.re.kr)
 */

#include <stdio.h>

#include "cmsis_os.h"

#include "ImC/imc_api.h"
#include "sc03mpd/sc03mpd.h"
#include "ImC/decoder.h"

/**
* @brief turn Set of 4 LEDs ON and OFF after msec
* @param argument:
* 			ledSet : indicates LED 0~3
* 				e.g. {1,0,1,0} for LED 0 and LED 2 are ON
* 			length : number of LEDs (ledSet)
* 			msec : ON duration
* @retval None
*/

void imc_led_on (uint8_t ledSet[], uint8_t length, uint32_t msec)
{
	HAL_GPIO_WritePin(LED0_GPIO, LED0_GPIO_PIN, GPIO_PIN_RESET);	// LED0 OFF
	HAL_GPIO_WritePin(LED1_GPIO, LED1_GPIO_PIN, GPIO_PIN_RESET);	// LED1 OFF
	HAL_GPIO_WritePin(LED2_GPIO, LED2_GPIO_PIN, GPIO_PIN_RESET);	// LED2 OFF
	HAL_GPIO_WritePin(LED3_GPIO, LED3_GPIO_PIN, GPIO_PIN_RESET);	// LED3 OFF

	for ( uint8_t i = 0; i < length ; i++ ) {
		if ( ledSet[i] )
		{
			switch (i) {	// LED ON
			  case 0: HAL_GPIO_WritePin(LED0_GPIO, LED0_GPIO_PIN, GPIO_PIN_SET); break;
			  case 1: HAL_GPIO_WritePin(LED1_GPIO, LED1_GPIO_PIN, GPIO_PIN_SET); break;
			  case 2: HAL_GPIO_WritePin(LED2_GPIO, LED2_GPIO_PIN, GPIO_PIN_SET); break;
			  case 3: HAL_GPIO_WritePin(LED3_GPIO, LED3_GPIO_PIN, GPIO_PIN_SET); break;
			}
		}
	}
	osDelay(msec);

	HAL_GPIO_WritePin(LED0_GPIO, LED0_GPIO_PIN, GPIO_PIN_RESET);	// LED0 OFF
	HAL_GPIO_WritePin(LED1_GPIO, LED1_GPIO_PIN, GPIO_PIN_RESET);	// LED1 OFF
	HAL_GPIO_WritePin(LED2_GPIO, LED2_GPIO_PIN, GPIO_PIN_RESET);	// LED2 OFF
	HAL_GPIO_WritePin(LED3_GPIO, LED3_GPIO_PIN, GPIO_PIN_RESET);	// LED3 OFF
}

/**
* @brief turn Set of 4 LEDs ON and OFF after msec
* @param argument:
* 			ledSet : indicates LED 0~3
* 				e.g. {1,0,1,0} for LED 0 and LED 2 are ON
* 			length : number of LEDs (ledSet)
* 			msec : blink period
* @retval None
*/
void imc_led_blink (uint8_t ledSet[], uint8_t length, uint32_t msec)
{
	imc_led_on (ledSet, length, msec);
	osDelay(msec);
}

int32_t imc_sc03mpd_init (sc03mpd_ifx_t* ifx)
{
	/* initial operation process :
	 * 1) power up
	 * 2) delay 2.5s
	 * 3) reset command
	 * 4) set image resolution command (currently 160x120)
	 * 5) set image compressibility command
	 * 6) reset command
	 *    Note: after setting the desired image size, you need to reset the camera first,
	 *    and then the new setting would be enabled.
	 * OPTION for SKKU) manually step color, select black-white
	 */


/*	//move to main.c
#if !CAM_FAST_READY
	printf("[CAM] wait %dms for boot-up\r\n", DELAY_AFTER_POWERUP);
	osDelay (DELAY_AFTER_POWERUP);
#endif
	//printf("[SC03MPD] init CAM ...\r\n");

	MX_USART3_UART_Init();
*/
	SC03MPD_ASSERT(sc03mpd_reset(ifx), "[CAM] FAILED reset", INIT_ERROR)
	SC03MPD_ASSERT(sc03mpd_set_ires(ifx, IMAGE_RESOLUTION),	"[CAM] FAILED set resolution", INIT_ERROR)
	SC03MPD_ASSERT(sc03mpd_reset(ifx), "[CAM] FAILED reset for image setting", INIT_ERROR)
	SC03MPD_ASSERT(sc03mpd_set_icmp(ifx, 0x00), "[CAM] FAILED set compression rate", INIT_ERROR)
#if CAM_BLACK_WHITE
	// after reset color mode setting is initialized
	SC03MPD_ASSERT(sc03mpd_reset(ifx), "[CAM] FAILED reset for image setting", INIT_ERROR)
	SC03MPD_ASSERT(sc03mpd_set_clr(ifx, SC03MPD_CLR_BW), "[CAM] FAILED set black-white", INIT_ERROR)
#endif

	printf("[CAM] OK init\r\n");

	return CAM_STS_ENON;

INIT_ERROR:
	return CAM_STS_EINI;
}

uint16_t ilen = 0;

uint8_t buf[4096];

/* 2022. 8. 25. fix logical seq. error under V0706 protocol; stop -> read length -> read data -> resume */
int32_t imc_sc03mpd_capture (sc03mpd_ifx_t* ifx) {
	//printf("[SC03MPD] start capture ...\r\n");

	/* CAPTURE */
	SC03MPD_ASSERT(sc03mpd_capture(ifx), "[CAM] FAILED capture", CAPTURE_ERROR)
	osDelay (DELAY_AFTER_CAPTURE);

	/* GET length */
	ilen = 0;
	SC03MPD_ASSERT(sc03mpd_get_ilen(ifx, &ilen), "[CAM] FAILED get length", CAPTURE_ERROR)
	printf("[CAM] OK capture (length = %d, %04X)\r\n", ilen, ilen);
	osDelay (DELAY_AFTER_LENGTH);

	/* READ image data */
	//printf("[SC03MPD] download to 0x%08X\r\n", IMG_RAM_ADDR);

	// uint8_t* imem = (uint8_t*)IMG_RAM_ADDR;
	uint8_t* imem = (uint8_t*)buf;
	uint16_t addr = 0;
	uint16_t nreq = 0;
	uint16_t rest = ilen;

	uint32_t tsStart = 0;
	uint32_t tsEnd = 0;

	tsStart = HAL_GetTick();
	while (rest > 0)
	{
	  nreq = (rest > IMG_BLK_SIZE)? IMG_BLK_SIZE : rest;

	  if (sc03mpd_get_idat(ifx, addr, nreq, imem))
	  {
		printf("[CAM] FAILED download: %04X (%d)\r\n", addr, nreq);
		goto CAPTURE_ERROR;
	  }

	  //printf ("#READ: %04X (%d), DATA: %02X\r\n", addr, nreq, *imem);
	  rest -= nreq;
	  addr += nreq;
	  imem += nreq;

	  //osDelay(DELAY_DURING_DOWNLOAD);
	}

	tsEnd = HAL_GetTick();
	printf("[CAM] OK download; %lu ticks, 0x%08X to 0x%08X\r\n", tsEnd-tsStart, IMG_RAM_ADDR, IMG_RAM_ADDR+ilen);

#if CAM_IMAGE_FIRST_TWO_BYTES
	/* Note: JPEG IMAGE DATA must be FF D8 in first and FF D9 in end.
	 *       2022. 7. 20. ETRI can not assure that the EOI is FF D9.
	 * */

	printf("[SC03MPD] image data (first 2 bytes): ");
	uint8_t* aimg = (uint8_t*)IMG_RAM_ADDR;
	for ( uint16_t i = 0; i < 2; i++, aimg++ )
		printf("%02X ", *aimg);

	printf("\r\n");
	printf("\r\n");
#endif

	/* STOP CAPTURE = RESUME */
	SC03MPD_ASSERT(sc03mpd_stop(ifx), "[CAM] FAILED stop", CAPTURE_ERROR)

	printf("[CAM] OK stop capture\r\n");
	return CAM_STS_ENON;

	CAPTURE_ERROR:
	return CAM_STS_ECAP;
}

int32_t imc_sc03mpd_reset (sc03mpd_ifx_t* ifx)
{
	SC03MPD_ASSERT(sc03mpd_reset(ifx), "[CAM] FAILED reset", RESET_ERROR)
	printf("[CAM] OK reset\r\n");

	return CAM_STS_ENON;

RESET_ERROR:
	return CAM_STS_EINI;
}


void imc_sbc_power_off ()
{
	printf("[EPS] SBC OFF\r\n");
	HAL_GPIO_WritePin(SBC_POWER_GPIO, SBC_POWER_GPIO_PIN, POWER_OFF_SBC);
}

void imc_sbc_power_on ()
{
	printf("[EPS] SBC on\r\n");
	HAL_GPIO_WritePin(SBC_POWER_GPIO, SBC_POWER_GPIO_PIN, POWER_ON_SBC);
}

void imc_cam_power_off ()
{
	printf("[EPS] CAM OFF\r\n");
	HAL_GPIO_WritePin(CAM_POWER_GPIO, CAM_POWER_GPIO_PIN, POWER_OFF);
}

void imc_cam_power_on ()
{
	printf("[EPS] CAM ON\r\n");
	HAL_GPIO_WritePin(CAM_POWER_GPIO, CAM_POWER_GPIO_PIN, POWER_ON);
}

#if (EPS_CAP_VOLT_GPIO)
/*
 * energy level : 0~7 with 3bits of GPIO pins
 * ignore level update between reading each pin
 *
 * (2022.11.10.) read the level three times (per 3ms), and return lower level
 *
 */
uint8_t imc_get_energy_level ()
{
	uint8_t curr = 8;
	uint8_t bigger = 0;

	for (uint8_t i = 0; i < 3; i ++)
	{
		curr	= (HAL_GPIO_ReadPin(ENERGY_LV0_GPIO, ENERGY_LV0_GPIO_PIN)
				+ (HAL_GPIO_ReadPin(ENERGY_LV1_GPIO, ENERGY_LV1_GPIO_PIN) << 1)
				+ (HAL_GPIO_ReadPin(ENERGY_LV2_GPIO, ENERGY_LV2_GPIO_PIN) << 2));

		if ( (curr > bigger) && (curr > 0) )
			bigger = curr;

		osDelay(DELAY_GET_ENERGY_LEVEL);
	}

	return bigger;
}
#endif

#if (EPS_CAP_VOLT_ADC)
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int medianCalc(int* input, int arr_size){
    qsort(input, arr_size, sizeof(int), compare);
    if (arr_size%2)
    	return input[arr_size/2];
    else
    	return (input[arr_size/2-1] + input[arr_size/2])/2;

}

uint16_t MEAS_COEF[2] = {812, 782};	// for EPS CAP ch0 and ch1
int measure_voltage(ADC_HandleTypeDef hadc, uint8_t ch){
	int res_value[5];

/* UCR ref.
	#define MEAS_COEF 3051	// UCR 2790, ETRI 3051
	int v_value = ((uint32_t)medianCalc(res_value, 5))*5000/MEAS_COEF; // in milliVolt
*/
	for (int i=0; i<5; i++)
	{
		HAL_ADC_Start(&hadc);
		// Poll ADC Peripheral & TimeOut = 1mSec
		HAL_ADC_PollForConversion(&hadc, 1);
		res_value[i] = HAL_ADC_GetValue(&hadc);
	}

	int v_value = ((uint32_t)medianCalc(res_value, 5)) * MEAS_COEF[ch] / 500; //
//	int voltage_d = v_value/1000;
//	int voltage_f = v_value%1000;
//	printf("ADC Value:     %d.%03d \r\n", voltage_d, voltage_f);
	return v_value;
}

int32_t imc_sc03mpd_cap_check (ADC_HandleTypeDef hadc)
{
	int count = 0;
	int capacitor_voltage = measure_voltage(hadc, EPS_CAP_ID_CAM);

	if (capacitor_voltage < EPS_CAP_VOLT_LOW_THRESHOLD_CAM)
	{
		imc_cam_power_off();
		printf ("[CAM] CAP#%d < %dmV\r\n", EPS_CAP_ID_CAM, EPS_CAP_VOLT_LOW_THRESHOLD_CAM);

		while (capacitor_voltage < EPS_CAP_VOLT_HIGH_THRESHOLD_CAM) {
			printf ("\twait(%2d) %dmV\r\n", count, capacitor_voltage);
			osDelay (DELAY_WAIT_CAP_CHECK);

			if (count >= COUNTER_WAIT_CAP_CHECK){
				return CAM_STS_EINI;
			}

			capacitor_voltage = measure_voltage(hadc, EPS_CAP_ID_CAM);
			count++;
		}

		printf ("[CAM] CAP#%d > %dV\r\n", EPS_CAP_ID_CAM, EPS_CAP_VOLT_HIGH_THRESHOLD_CAM);
	}

	return CAM_STS_ENON;
}
#endif
