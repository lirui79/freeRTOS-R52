#ifndef _IO_H_
#define _IO_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef __force
#define __force
#endif

#define __raw_readb(a)			(*(volatile unsigned char *)(a))
#define __raw_readw(a)			(*(volatile unsigned short *)(a))
#define __raw_readl(a)			(*(volatile unsigned int *)(a))
//#define __raw_readq(a)			(*(volatile unsigned long long *)(a))

#define __raw_writeb(v, a)		(*(volatile unsigned char *)(a) = (v))
#define __raw_writew(v, a)		(*(volatile unsigned short *)(a) = (v))
#define __raw_writel(v, a)		(*(volatile unsigned int *)(a) = (v))
//#define __raw_writeq(v, a)		(*(volatile unsigned long long *)(a) = (v))

#define writeb(v, c)	({ uint8_t  __v = v; __raw_writeb(__v, c); __v; })
#define writew(v, c)	({ uint16_t __v = v; __raw_writew(__v, c); __v; })
#define writel(v, c)	({ uint32_t __v = v; __raw_writel(__v, c); __v; })
//#define writeq(v, c)	({ uint64_t __v = v; __raw_writeq(__v, c); __v; })

#define readb(c)	({ uint8_t  __v = __raw_readb(c); __v; })
#define readw(c)	({ uint16_t __v = __raw_readw(c); __v; })
#define readl(c)	({ uint32_t __v = __raw_readl(c); __v; })
//#define readq(c)	({ uint64_t __v = __raw_readq(c); __v; })


#endif
