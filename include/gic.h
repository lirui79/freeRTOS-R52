#ifndef GIC_H
#define GIC_H

#include "FreeRTOS.h"

#define gic_acc(off) \
	(*((volatile uint32_t*)(off)))
#define gic_read(off, var) \
	var = *((volatile uint32_t*)(off))
#define gic_write(off, val) \
	*((volatile uint32_t*)(off)) = (val)
#define gic_set(off, bit) \
	*((volatile uint32_t*)(off)) |= (bit)

//#define GIC_DIST_BASE			0x50800000
#define GIC_DIST_BASE			0xf0000000
#define GICD_IROUTER_OFFSET 	0x6000

#define GIC_GICD_IROUTER(n)	(GIC_DIST_BASE + GICD_IROUTER_OFFSET + ( (n) * 8 ) )	/* Interrupt Processor Targets Registers */
#define REG_GIC_GICD_IROUTER(n)     ((volatile uint64_t *)(uintptr_t)GIC_GICD_IROUTER(n))

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

void gic_setup_interrupt(uint32_t irq_id);
void gic_setup(void);
uint32_t gic_get_irq(void);
void gic_ack_irq(uint32_t irq_id);

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif
