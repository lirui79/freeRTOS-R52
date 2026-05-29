#include "stdbool.h"
#include "gic.h"

void set_gic_irq_router(uint32_t irq_id, uint32_t affinity, bool irm_mode) {
	uint64_t route_val;
    
	if (irq_id < 32) {
        return; 
    }

    uint64_t irouter_reg = *REG_GIC_GICD_IROUTER(irq_id * 8);    
    
    if (irm_mode) {
        route_val = (1ULL << 31);
    } else {
        route_val = (uint64_t)affinity & ~(1ULL << 31);
    }

	*REG_GIC_GICD_IROUTER(irq_id * 8) = route_val;
}

void gic_setup_interrupt(uint32_t irq_id)
{
	uint32_t affinity = 0;
	
	//set IGROUPRn
	gic_set(GIC_DIST_BASE + 0x80 + (irq_id/32)*4, (1 << (irq_id % 32)));//put irq into group1
	
	// set ISENABELRn
	gic_set(GIC_DIST_BASE + 0x100 + (irq_id/32)*4, (1 << (irq_id % 32)));//enable irq 
	
	// set ITARGETSn or IROUTER
	//gic_set(GIC_DIST_BASE + 0x800 + (irq_id/4)*4, (0x01 << 8*(irq_id%4)));
	set_gic_irq_router(irq_id, affinity, 0);
	
	// set ICFGRn to level sensitive
	// TODO should be bitclear
	gic_write(GIC_DIST_BASE + 0xc00 + (irq_id/16)*4, 0);
}

void gic_setup(void)
{
	// we need to use system register access. uvmm does not provide mmio for cpu
	// interface
	asm volatile("mcr p15, 0, %0, c12, c12, 5" // ICC_SRE
			   : : "r" (0x7));

	// timer interrupt: dts irq 11 -> irq_id 27
	gic_setup_interrupt(27);

	// enable Group 1 interrupts
	asm volatile("mcr p15, 0, %0, c12, c12, 7" : : "r" (1)); // ICC_IGRPEN1

	// set ICC_PRM
	asm volatile("mcr p15, 0, %0, c4, c6, 0" : : "r"(0xff));

	// configure distributor control register, enable group 1NS irqs
	gic_write(GIC_DIST_BASE, 0x2);
}

uint32_t gic_get_irq(void)
{
  // read IAR1 for group1
  uint32_t v;
  asm volatile("mrc p15, 0, %0, c12, c12, 0" : "=r"(v));
  return v;
}

void gic_ack_irq(uint32_t irq_id)
{
	// set EOIR for group1
	asm volatile("mcr p15, 0, %0, c12, c12, 1" : : "r"(irq_id));
}
