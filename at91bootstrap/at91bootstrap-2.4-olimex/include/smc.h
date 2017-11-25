#define	MASTER_CLOCK_CYCLE	(1000000000/MASTER_CLOCK)
#define	_ns(x)	((x)/MASTER_CLOCK_CYCLE)

#define CONTROLLED_BY_NRD	AT91C_SMC_READMODE
#define CONTROLLED_BY_NCS_RD	0
#define CONTROLLED_BY_NWE	AT91C_SMC_WRITEMODE
#define CONTROLLED_BY_NCS_WR	0

#define TDF_OPTIMIZATION_ON	AT91C_SMC_TDFEN
#define TDF_OPTIMIZATION_OFF	0

#define PAGE_MODE_ON		AT91C_SMC_PMEN
#define PAGE_MODE_OFF		0

#define	DATABUS_PULLUP_ON	0
#define	DATABUS_PULLUP_OFF	AT91C_EBI_DBPUC

#define	MEMORY_VOLTAGE_1_8V	0
#define	MEMORY_VOLTAGE_3_3V	AT91C_EBI_VDDIOMSEL