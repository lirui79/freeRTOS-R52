//#include "type.h"
//#include "io.h"
//#include "common.h"
//#include "sysmap.h"
#include <stdint.h>
#include "uart-dw-apb.h"

#define UART_BASE           0x59100000

#define UART0 (UART_BASE + 0 * 0x4000)

#define LSR_TX_IDLE    (1 << 5)    // THR can accept another character to send
#define LSR_DATA_READY (1 << 0)    // FIFO is non-empty

#define LCR_DIVISOR_LATCH_ACCESS  (1 << 7)

#define readl(reg)            (*((volatile uint32_t *)(reg)))
#define writel(val, reg)	do { *((volatile uint32_t *)reg) = val; } while (0)
	
#define read(reg) readl(UART0 + reg)
#define write(reg, val) writel(val, UART0 + reg)

// uart_init does a machine-specific initialization of UART.
void uart_init() {
    write(0x0, UART_IER);      // disable all interrupts
    write(0xf7, UART_FCR);     // reset FIFO

    // The manual says to "Set the flow control parameter by writing the
    // UART_MCR register", which probably implies bit 5, the "auto flow
    // control". I don't know what that does, however, and it doesn't seem to be
    // necessary:
    // write(0x0, UART_MCR);

    // This bit is very weird: if I execute the block below, I get an infinite
    // barrage of UART interrupts. PLIC just keeps firing with UART0_IRQ_NUM,
    // but nothing in the UART itself indicates anything actionable is going on.
    // And when I mark the interrupt as complete, PLIC immediately fires again,
    // bogging down the CPU.
    /*
    uint32_t val = read(UART_LCR);
    val |= LCR_DIVISOR_LATCH_ACCESS;    // select Divisor Latch Register
    write(val, UART_LCR);
    write(0xd & 0xff, UART_DLL);        // 0x0d=13 240000000/(13*16) = 115200 Divisor Latch Low
    // write((0xd >> 8) & 0xff, UART_DLH); // Divisor Latch High
    write(0, UART_DLH); // Divisor Latch High
    val = read(UART_LCR);
    val &= ~LCR_DIVISOR_LATCH_ACCESS;
    write(val, UART_LCR);

    uint32_t val = read(UART_LCR);
    val &= ~0x1f;
    val |= (0x3 << 0) | (0 << 2) | (0 << 3); // 8 bit, 1 stop bit, parity disabled
    write(val, UART_LCR);
    */

    write(1, UART_FCR); // enable FIFOs
    write(1, UART_IER); // only enable RX IRQ

    //plic_set_intr_priority(UART0_IRQ_NUM, PLIC_MAX_PRIORITY);
    //plic_set_threshold(PLIC_MAX_PRIORITY - 1);
    //plic_enable_intr(UART0_IRQ_NUM);
}

void uart_writechar(char ch) {
    while ((read(UART_LSR) & LSR_TX_IDLE) == 0)
        ;
    write(UART_THR, ch);
}

char uart_readchar() {
    return read(UART_RBR);
}

int uart_rx_num_avail() {
    uint32_t rfl = read(UART_RFL);
    return rfl;
}

void uart_machine_wait_status() {
    // no-op
}

void low_uart_put(int ch) {
    uart_writechar(ch);
}

void platform_putc(uint8_t ch){
    uart_writechar((char)ch);
}

#if 1
void _putchar(char character)
{
    if (character == '\n')
        low_uart_put('\r');
    low_uart_put(character);
}
#endif
