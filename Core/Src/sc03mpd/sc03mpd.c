/*
 * sc03mpd.c
 *
 *  Created on: Jul 12, 2022
 *      Author: jylew
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sc03mpd/sc03mpd.h"

#define SC03MPD_UTL_NUSD(v)     (void)(v)
#define SC03MPD_UTL_V_HI(v)     ((uint8_t)(((v) & 0xFF00) >> 8))
#define SC03MPD_UTL_V_LO(v)     ((uint8_t)(((v) & 0x00FF) >> 0))


#define VC0706_PSN_REQ          (0x56)
#define VC0706_PSN_REP          (0x76)

#define SC03MPD_CMD_BDR         (0x24) // SET BAUD RATE
#define SC03MPD_CMD_RST         (0x26) // RESET
#define SC03MPD_CMD_CMP         (0x31) // SET IMAGE COMPRESSION RATE
#define SC03MPD_CMD_RES         (0x31) // SET IMAGE RESOLUTION
#define SC03MPD_CMD_CDB         (0x31) // CHANGE DEFAULT BAUD RATE
#define SC03MPD_CMD_IMG         (0x32) // READ IMAGE DATA
#define SC03MPD_CMD_LEN         (0x34) // READ IMAGE DATA LENGTH
#define SC03MPD_CMD_CTR         (0x36) // CAPTURE IMAGE
#define SC03MPD_CMD_STP         (0x36) // STOP CAPTURE
#define SC03MPD_CMD_CLR         (0x3C) // COLOR CONTROL MODE AND SHOW MODE

#define SC03MPD_PKT_REQ_LEN     (4)
#define SC03MPD_PKT_REP_LEN     (5)
#define SC03MPD_PKT_DAT_LEN     (16)
#define SC03MPD_PKT_MAX_MTU     (SC03MPD_PKT_REP_LEN + SC03MPD_PKT_DAT_LEN)


#pragma pack(push, 1)
typedef struct
{
    uint8_t psn;    // Protocol Sign
    uint8_t dsn;    // Device Serial Number
    uint8_t cmd;    // Command
    uint8_t len;    // Data length
    uint8_t dat[0];
}
sc03mpd_req_t;

typedef struct
{
    uint8_t psn;    // Protocol Sign
    uint8_t dsn;    // Device Serial Number
    uint8_t cmd;    // Command
    uint8_t sts;    // Status
    uint8_t len;    // Data length
    uint8_t dat[0];
}
sc03mpd_rep_t;

typedef union
{
    sc03mpd_req_t req_hdr;
    sc03mpd_rep_t rep_hdr;
    uint8_t       pkt_buf[SC03MPD_PKT_MAX_MTU];
}
sc03mpd_transaction_t;
#pragma pack(pop)

typedef void (*sc03mpd_cmd_builder)(sc03mpd_transaction_t*, uint16_t, uint16_t);



static void sc03mpd_init_trx(sc03mpd_transaction_t* trx, uint8_t cmd, uint8_t len)
{
    trx->req_hdr.psn = VC0706_PSN_REQ;
    trx->req_hdr.dsn = SC03MPD_DSN_DEF;
    trx->req_hdr.cmd = cmd;
    trx->req_hdr.len = len;
}

static void sc03mpd_build_rst(sc03mpd_transaction_t* trx, uint16_t arg0, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg0);
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_RST, 0);
}

static void sc03mpd_build_bdr(sc03mpd_transaction_t* trx, uint16_t baud, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_BDR, 3);
    trx->req_hdr.dat[0] = 0x01;
    trx->req_hdr.dat[1] = SC03MPD_UTL_V_HI(baud);
    trx->req_hdr.dat[2] = SC03MPD_UTL_V_LO(baud);
}

static void sc03mpd_build_cdb(sc03mpd_transaction_t* trx, uint16_t baud, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_CDB, 6);
    trx->req_hdr.dat[0] = 0x04;
    trx->req_hdr.dat[1] = 0x02;
    trx->req_hdr.dat[2] = 0x00;
    trx->req_hdr.dat[3] = 0x08;
    trx->req_hdr.dat[4] = SC03MPD_UTL_V_HI(baud);
    trx->req_hdr.dat[5] = SC03MPD_UTL_V_LO(baud);
}

static void sc03mpd_build_cmp(sc03mpd_transaction_t* trx, uint16_t cmpr, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_CMP, 5);
    trx->req_hdr.dat[0] = 0x01;
    trx->req_hdr.dat[1] = 0x01;
    trx->req_hdr.dat[2] = 0x12;
    trx->req_hdr.dat[3] = 0x04;
    trx->req_hdr.dat[4] = (uint8_t)cmpr;
}

static void sc03mpd_build_res(sc03mpd_transaction_t* trx, uint16_t resn, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_RES, 5);
    trx->req_hdr.dat[0] = 0x04;
    trx->req_hdr.dat[1] = 0x01;
    trx->req_hdr.dat[2] = 0x00;
    trx->req_hdr.dat[3] = 0x19;
    trx->req_hdr.dat[4] = (uint8_t)resn;
}

static void sc03mpd_build_ctr(sc03mpd_transaction_t* trx, uint16_t arg0, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg0);
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_CTR, 1);
    trx->req_hdr.dat[0] = 0x00;
}

static void sc03mpd_build_stp(sc03mpd_transaction_t* trx, uint16_t arg0, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg0);
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_STP, 1);
    trx->req_hdr.dat[0] = 0x03;		/* in case of two images capture, this parameter should be 0x02 */
}

static void sc03mpd_build_len(sc03mpd_transaction_t* trx, uint16_t arg0, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg0);
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_LEN, 1);
    trx->req_hdr.dat[0] = 0x00;
}

static void sc03mpd_build_img(sc03mpd_transaction_t* trx, uint16_t addr, uint16_t size)
{
    sc03mpd_init_trx(trx, SC03MPD_CMD_IMG, 12);
    trx->req_hdr.dat[ 0] = 0x00;
    trx->req_hdr.dat[ 1] = 0x0A;
    trx->req_hdr.dat[ 2] = 0x00;
    trx->req_hdr.dat[ 3] = 0x00;
    trx->req_hdr.dat[ 4] = SC03MPD_UTL_V_HI(addr);
    trx->req_hdr.dat[ 5] = SC03MPD_UTL_V_LO(addr);
    trx->req_hdr.dat[ 6] = 0x00;
    trx->req_hdr.dat[ 7] = 0x00;
    trx->req_hdr.dat[ 8] = SC03MPD_UTL_V_HI(size);
    trx->req_hdr.dat[ 9] = SC03MPD_UTL_V_LO(size);
    trx->req_hdr.dat[10] = 0x00;
    trx->req_hdr.dat[11] = 0x0A;
}

static void sc03mpd_build_clr(sc03mpd_transaction_t* trx, uint16_t showMode, uint16_t arg1)
{
    SC03MPD_UTL_NUSD(arg1);

    sc03mpd_init_trx(trx, SC03MPD_CMD_CLR, 2);
    trx->req_hdr.dat[0] = 0x01;
    trx->req_hdr.dat[1] = (uint8_t)showMode;
}

static int32_t sc03mpd_do_transaction(sc03mpd_ifx_t* ifx, sc03mpd_transaction_t* trx, sc03mpd_cmd_builder builder, uint16_t arg0, uint16_t arg1)
{
    builder(trx, arg0, arg1);

    uint8_t dsn = trx->req_hdr.dsn;
    uint8_t cmd = trx->req_hdr.cmd;
    uint8_t txn = sizeof(sc03mpd_req_t) + trx->req_hdr.len;
    uint8_t rxn = sizeof(sc03mpd_rep_t);

    if (ifx->sendif(ifx, trx->pkt_buf, txn) != txn)
        return VC0706_STS_ECOM;

    if (ifx->recvif(ifx, trx->pkt_buf, rxn) != rxn)
    	return VC0706_STS_ECOM;

    if (trx->rep_hdr.dsn != dsn) return VC0706_STS_ECOM;
    if (trx->rep_hdr.cmd != cmd) return VC0706_STS_ECOM;
    if (trx->rep_hdr.psn != VC0706_PSN_REP ) return VC0706_STS_ECOM;
    if (trx->rep_hdr.sts != VC0706_STS_ENON) return -((int32_t)trx->rep_hdr.sts);

    uint8_t rxd = trx->rep_hdr.len;
    if (rxd > 0)
    {
        if (ifx->recvif(ifx, &trx->pkt_buf[rxn], rxd) != rxd)
            return VC0706_STS_ECOM;
    }

    return VC0706_STS_ENON;
}

static int32_t sc03mpd_validate_baud(uint16_t baud)
{
    switch (baud)
    {
        case SC03MPD_BDR_9600  :
        case SC03MPD_BDR_19200 :
        case SC03MPD_BDR_38400 :
        case SC03MPD_BDR_56700 :
        case SC03MPD_BDR_115200: return 0;
    }
    return 1;
}

static int32_t sc03mpd_validate_ires(uint8_t ires)
{
    switch (ires)
    {
        case SC03MPD_RES_640_480:
        case SC03MPD_RES_320_240:
        case SC03MPD_RES_160_120: return 0;
    }
    return 1;
}

static int32_t sc03mpd_validate_clr(uint8_t clr)
{
    switch (clr)
    {
        case SC03MPD_CLR_AUTO:
        case SC03MPD_CLR_COLOR:
        case SC03MPD_CLR_BW: return 0;
    }
    return 1;
}

int32_t sc03mpd_reset(sc03mpd_ifx_t* ifx)
{
    sc03mpd_transaction_t trx;
    if (sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_rst, 0, 0))
        return VC0706_STS_ECOM;

    // Some ASCII string follows after vc0706 response.
    uint8_t dbuf[192] = { 0, };
    int32_t nrxd = 0;
	int32_t accq = 0;
	int32_t rest = sizeof(dbuf) - 1;

	while (rest > 0)
	{
		if ((nrxd = ifx->recvif(ifx, &dbuf[accq], rest)) <= 0)
			break;

		accq += nrxd;
		rest -= nrxd;
	}

#ifdef SHOW_RESET_RESULT
	printf("%s\n", dbuf);
#endif

    return VC0706_STS_ENON;
}

int32_t sc03mpd_set_baud(sc03mpd_ifx_t* ifx, uint16_t baud, uint8_t save)
{
    if (sc03mpd_validate_baud(baud))
        return VC0706_STS_EFMT;

    sc03mpd_transaction_t trx;
    sc03mpd_cmd_builder builder = (save)? sc03mpd_build_cdb : sc03mpd_build_bdr;

    return sc03mpd_do_transaction(ifx, &trx, builder, baud, 0);
}

int32_t sc03mpd_set_icmp(sc03mpd_ifx_t* ifx, uint8_t cmpr)
{
    sc03mpd_transaction_t trx;
    return sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_cmp, cmpr, 0);
}

int32_t sc03mpd_set_ires(sc03mpd_ifx_t* ifx, uint8_t ires)
{
    if (sc03mpd_validate_ires(ires))
        return VC0706_STS_EFMT;

    sc03mpd_transaction_t trx;
    return sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_res, ires, 0);
}

int32_t sc03mpd_get_ilen(sc03mpd_ifx_t* ifx, uint16_t* length)
{
    sc03mpd_transaction_t trx;

    if (sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_len, 0, 0))
        return VC0706_STS_ECOM;

    *length = ((uint16_t)trx.rep_hdr.dat[2] << 8) |
              ((uint16_t)trx.req_hdr.dat[3] << 0);

    return VC0706_STS_ENON;
}

int32_t sc03mpd_get_idat(sc03mpd_ifx_t* ifx, uint16_t addr, uint16_t size, uint8_t* dbuf)
{
    if (dbuf == NULL) return VC0706_STS_EINT;
    if (size == 0   ) return VC0706_STS_EINT;

    sc03mpd_transaction_t trx;
    if (sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_img, addr, size))
        return VC0706_STS_ECOM;

    int32_t nrxd = 0;
    int32_t accq = 0;
    int32_t rest = size;
    while (rest > 0)
    {
        if ((nrxd = ifx->recvif(ifx, &dbuf[accq], rest)) <= 0)
            return VC0706_STS_ECOM;

        accq += nrxd;
        rest -= nrxd;
    }

    sc03mpd_rep_t res;
    if (ifx->recvif(ifx, (uint8_t*)&res, sizeof(res)) != sizeof(res))
        return VC0706_STS_ECOM;

    if (res.psn != trx.rep_hdr.psn) return VC0706_STS_ECOM;
    if (res.dsn != trx.rep_hdr.dsn) return VC0706_STS_ECOM;
    if (res.cmd != trx.rep_hdr.cmd) return VC0706_STS_ECOM;
    if (res.sts != trx.rep_hdr.sts) return VC0706_STS_ECOM;
    if (res.len != trx.rep_hdr.len) return VC0706_STS_ECOM;

    return VC0706_STS_ENON;
}

int32_t sc03mpd_capture(sc03mpd_ifx_t* ifx)
{
    sc03mpd_transaction_t trx;
    return sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_ctr, 0, 0);
}

int32_t sc03mpd_stop(sc03mpd_ifx_t* ifx)
{
    sc03mpd_transaction_t trx;
    return sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_stp, 0, 0);
}

int32_t sc03mpd_set_clr(sc03mpd_ifx_t* ifx, uint8_t showMode)
{
	if (sc03mpd_validate_clr(showMode))
	        return VC0706_STS_EFMT;

    sc03mpd_transaction_t trx;
    return sc03mpd_do_transaction(ifx, &trx, sc03mpd_build_clr, showMode, 0);
}
