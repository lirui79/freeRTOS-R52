#ifndef _UART_H_
#define _UART_H_

#define putc cdns_uart_poll_put_char
#define puts cdns_uart_poll_puts

void uart_init();
char uart_readchar();
void uart_writechar(char ch);

#endif