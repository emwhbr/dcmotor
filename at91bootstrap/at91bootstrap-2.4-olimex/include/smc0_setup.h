/* ******************************************************************* */
/* SMC0 Settings                                                       */
/* We use this as a default for all				       */
/* ******************************************************************* */

#ifndef _SMC0_SETUP_H
#define _SMC0_SETUP_H

#define	NCS_READ_SETUP	_ns(10)
#define	NRD_SETUP	_ns(20)
#define	NCS_WR_SETUP	_ns(10)
#define	NWE_SETUP	_ns(20)

#define	NCS_READ_PULSE	_ns(100)
#define	NRD_PULSE	_ns(100)
#define	NCS_WR_PULSE	_ns(100)
#define	NWE_PULSE	_ns(100)

#define	NRD_CYCLE	_ns(140)
#define	NWE_CYCLE	_ns(140)

#define PS		AT91C_SMC_PS_SIZE_SIXTEEN_BYTES
#define PMEN		PAGE_MODE_OFF
#define TDF_MODE	TDF_OPTIMIZATION_OFF
#define TDF_CYCLES	_ns(60)
#define DBW		AT91C_SMC_DBW_WIDTH_SIXTEEN_BITS
#define BAT		AT91C_SMC_BAT_BYTE_SELECT
#define EXNW_MODE	AT91C_SMC_NWAITM_NWAIT_DISABLE
#define WRITE_MODE	CONTROLLED_BY_NWE
#define READ_MODE	CONTROLLED_BY_NRD

#endif
