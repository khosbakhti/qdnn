/*
 * uartBufferIT.h
 *
 *  Created on: Aug 22, 2022
 *      Author: yoojin.lim
 *
 *  1. Current MAX buffer size : 65535 (ref. range of uint16)
 *  2. Consider DMA and circular queue if you need data read over 65535 bytes
 */

#ifndef INC_IMC_UARTBUFFERIT_H_
#define INC_IMC_UARTBUFFERIT_H_

#include <stdint.h>

extern		UART_HandleTypeDef	huart2;

#define		UART				huart2
#define		UART_INSTANCE		USART2
#define		RX_BUFFER_SIZE		65535	//	uint16 range 0 ~ 65535

void uartBufferInit (void);
void uartBufferReset (void);

#endif /* INC_IMC_UARTBUFFERIT_H_ */
