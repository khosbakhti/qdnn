/*
 * imc_api.h
 *
 *  Created on: Jul 21, 2022
 *      Author: yoojin.lim d(yoojin.lim@etri.re.kr)
 */

#ifndef INC_IMC_IMC_API_H_
#define INC_IMC_IMC_API_H_

#include "stm32l5xx_hal.h"
#include "sc03mpd/sc03mpd.h"

#define UART_HANDLER_SBC		huart1
#define UART_HANDLER_CAM		huart3

#define NUM_OF_LED				4

#define LED0_GPIO				GPIOB
#define LED1_GPIO				GPIOB
#define LED2_GPIO				GPIOC
#define LED3_GPIO				GPIOC

#define LED0_GPIO_PIN			GPIO_PIN_1
#define LED1_GPIO_PIN			GPIO_PIN_2
#define LED2_GPIO_PIN			GPIO_PIN_6
#define LED3_GPIO_PIN			GPIO_PIN_7

#define SBC_POWER_GPIO			GPIOG
#define CAM_POWER_GPIO			GPIOC

#define SBC_POWER_GPIO_PIN		GPIO_PIN_11
#define CAM_POWER_GPIO_PIN		GPIO_PIN_7

#define POWER_ON_SBC			GPIO_PIN_RESET		// LOW; only for SBC
#define POWER_OFF_SBC			GPIO_PIN_SET		// HIGH; only for SBC
#define POWER_ON				GPIO_PIN_SET		// CAM, Sensors, etc. (KETI, 230811)
#define POWER_OFF				GPIO_PIN_RESET		// CAM, Sensors, etc. (KETI, 230811)

#define EPS_CAP_VOLT_GPIO		0
#define EPS_CAP_VOLT_ADC		1

#if (EPS_CAP_VOLT_GPIO)
#define EPS_SBC_OFF_LEVEL		1					// 1~7

#define ENERGY_LV0_GPIO			GPIOF
#define ENERGY_LV1_GPIO			GPIOF
#define ENERGY_LV2_GPIO			GPIOF

#define ENERGY_LV0_GPIO_PIN		GPIO_PIN_8
#define ENERGY_LV1_GPIO_PIN		GPIO_PIN_9
#define ENERGY_LV2_GPIO_PIN		GPIO_PIN_10
#endif

#if (EPS_CAP_VOLT_ADC)
#define ADC_HANDLER_SBC		hadc1
#define ADC_HANDLER_CAM		hadc2
#define EPS_CAP_VOLT_LOW_THRESHOLD_SBC	(3600) 		// Voltage threshold to do the checkpointing in MilliVolt
#define EPS_CAP_VOLT_LOW_THRESHOLD_CAM	(3700) 		// Voltage threshold to do the checkpointing in MilliVolt
#define EPS_CAP_VOLT_HIGH_THRESHOLD_CAM	(4800)		// Voltage threshold to do the checkpointing in MilliVolt
#define DELAY_WAIT_CAP_CHECK	(2000)
#define COUNTER_WAIT_CAP_CHECK	(60)
#define EPS_CAP_ID_SBC			0
#define EPS_CAP_ID_CAM			1

int measure_voltage(ADC_HandleTypeDef hadc, uint8_t ch);
int32_t imc_sc03mpd_cap_check (ADC_HandleTypeDef hadc);
#endif

#define	CAM_SHOT_PIN			GPIO_PIN_6

#define	DELAY_IGNORE_REQ		(500)				// 500ticks
#define DELAY_AFTER_POWERUP		(1000)				// 2500 ms in manual
#define DELAY_AFTER_CAPTURE		(250)				// 250
#define DELAY_AFTER_LENGTH		(10)				//
#define DELAY_DURING_DOWNLOAD	(1)
#define DELAY_BEFORE_ERR_HANDLING	(100)			// 50 ms
#define DELAY_GET_ENERGY_LEVEL	(3)					// 3 ms
#define DELAY_AFTER_WORK		(100)				// 50ms
#define DELAY_DO_SOMETHING		(1000)

#define IMG_RAM_ADDR			(0x68200000)
#define IMG_BLK_SIZE			(256)

#define CAM_STS_ENON			( 0)				// Executing right
#define CAM_STS_EINI			(-1)				// Error during init.
#define CAM_STS_ECAP			(-2)				// Error during image capturing
#define CAM_STS_EDWN			(-3)				// Error during image downloading

#define CAM_CHANGE_BAUDRATE			0				// set 1 and execute once when change CAM default baudrate
#define CAM_BAUDRATE_OPERATING		115200
#define CAM_BAUDRATE_CURRENT		38400
#define CAM_IMAGE_FIRST_TWO_BYTES	0
#define CAM_FAST_READY				0
#define CAM_BLACK_WHITE				0
#define CAM_COMP_ZERO				1
#define CAM_ERROR_COUNT_MAX			5				// successive ERRORs -> SBC reboot

#define IMAGE_RESOLUTION		SC03MPD_RES_160_120	// _640_480, _320_240 (default), _160_120
#if CAM_COMP_ZERO
#define IMAGE_COMPRESSIBILTY	0x00				// SKKU needs raw pixel format to feed an image to AI
#else
#define IMAGE_COMPRESSIBILTY	0x36				// 0x00~FF, 0x36 (default)
#endif

void imc_led_on (uint8_t ledSet[], uint8_t count, uint32_t msec);
void imc_led_blink (uint8_t ledSet[], uint8_t count, uint32_t msec);

int32_t imc_sc03mpd_init(sc03mpd_ifx_t* ifx);
int32_t imc_sc03mpd_capture (sc03mpd_ifx_t* ifx);
int32_t imc_sc03mpd_reset (sc03mpd_ifx_t* ifx);

void imc_sbc_power_off ();
void imc_sbc_power_on ();
void imc_cam_power_off ();
void imc_cam_power_on ();

uint8_t imc_get_energy_level ();

extern uint16_t ilen;
#endif /* INC_IMC_IMC_API_H_ */
