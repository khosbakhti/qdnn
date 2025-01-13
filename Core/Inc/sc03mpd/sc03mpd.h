/*
 * sc03mpd.h
 *
 *  Created on: Jul 12, 2022
 *      Author: jylew
 */

#ifndef INC_SC03MPD_SC03MPD_H_
#define INC_SC03MPD_SC03MPD_H_


/*
 *  Reference: SC03MPD User Manual (Rev.E)
 *             0.3 Mega Pixels Serial JPEG Camera with NTSC Video
 */

#include <stdint.h>


#define SC03MPD_DSN_DEF         (0x00) // Default Serial Number

#define VC0706_STS_ENON         ( 0) // Executing command right
#define VC0706_STS_EDNY         (-1) // System don't receive the command
#define VC0706_STS_ELEN         (-2) // The data-length is error
#define VC0706_STS_EFMT         (-3) // Data format error
#define VC0706_STS_ENAV         (-4) // The command can not execute now
#define VC0706_STS_EEXE         (-5) // Command received, but executed wrong
#define VC0706_STS_EINT         (-6) // Internal Error
#define VC0706_STS_ECOM         (-7) // Comm Error

#define SC03MPD_BDR_9600        (0xAEC8)
#define SC03MPD_BDR_19200       (0x56E4)
#define SC03MPD_BDR_38400       (0x2AF2)
#define SC03MPD_BDR_56700       (0x1C4C)
#define SC03MPD_BDR_115200      (0x0DA6)

#define SC03MPD_RES_640_480     (0x00)
#define SC03MPD_RES_320_240     (0x11)
#define SC03MPD_RES_160_120     (0x22)

#define SC03MPD_CLR_AUTO		(0x00)		// automatically step black-white and color
#define SC03MPD_CLR_COLOR		(0x02)		// manually step color, select color
#define SC03MPD_CLR_BW			(0x03)		// manually step color, select black-white

#define SC03MPD_ASSERT(func, msg, label)	\
	{ if (func) { printf("%s\r\n", msg); goto label; } }

struct sc03mpd_ifx;

typedef int32_t (*sc03mpd_ifx_fn)(struct sc03mpd_ifx* ifx, uint8_t* buf, uint16_t len);

typedef struct sc03mpd_ifx
{
    void* context;
    sc03mpd_ifx_fn sendif;
    sc03mpd_ifx_fn recvif;
}
sc03mpd_ifx_t;


/*
 * Refer to the chapter 11. 'INITIAL OPERATION PROCESS',
 * and chapter 12. 'Capture on image operation process'
 */


// Command Protocol: [1] RESET
int32_t sc03mpd_reset(sc03mpd_ifx_t* ifx);

// Command Protocol: [2] CAPTURE A IMAGE
int32_t sc03mpd_capture(sc03mpd_ifx_t* ifx);

// Command Protocol: [3] READ IMAGE DATA LENGTH
int32_t sc03mpd_get_ilen(sc03mpd_ifx_t* ifx, uint16_t* length);

// Command Protocol: [4] READ IMAGE DATA
int32_t sc03mpd_get_idat(sc03mpd_ifx_t* ifx, uint16_t addr, uint16_t size, uint8_t* dbuf);

// Command Protocol: [5] STOP CAPTURE
int32_t sc03mpd_stop(sc03mpd_ifx_t* ifx);

// Command Protocol: [6] SETTING IMAGE COMPRESSIBILITY
//   - cmpr : compression ratio (0 - 255)
//            = ([cmpr] - 1) * 0.25, if [cmpr] >= 1
//            = 0, otherwise
int32_t sc03mpd_set_icmp(sc03mpd_ifx_t* ifx, uint8_t cmpr);

// Command Protocol: [7] SETTING IMAGE RESOLUTION
//   - resn : image resolution
//            SC03MPD_RES_160_120
//            SC03MPD_RES_320_240
//            SC03MPD_RES_640_480

int32_t sc03mpd_set_ires(sc03mpd_ifx_t* ifx, uint8_t resn);

// Command Protocol: [8] BAUD RATE (if save = 0)
//                   [9] CHANGE DEFAULT BAUD RATE (if save > 0)
//   - baud : baud rate
//            SC03MPD_BDR_9600
//            SC03MPD_BDR_19200
//            SC03MPD_BDR_38400
//            SC03MPD_BDR_56700
//            SC03MPD_BDR_115200
int32_t sc03mpd_set_baud(sc03mpd_ifx_t* ifx, uint16_t baud, uint8_t save);

// Command Protocol from VC0706-Protocol: 1.3.2.17.COLOR_CTRL
//   - showMode : show different color by UART, it is effective only with UART
//            SC03MPD_CLR_AUTO
//            SC03MPD_CLR_COLOR
//            SC03MPD_CLR_BW
int32_t sc03mpd_set_clr(sc03mpd_ifx_t* ifx, uint8_t showMode);

#endif /* INC_SC03MPD_SC03MPD_H_ */
