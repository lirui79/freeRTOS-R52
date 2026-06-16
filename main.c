#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "printf.h"
#include "debug.h"
#include "gic.h"
#include "io.h"
#include "system.h"
#include "cmd.h"

#include "vcodec.h"

#include <mhu_v3_x_private.h>
#include <mhu_v3_x.h>
#include <mhu.h>

#define TEST_MAILBOX
#define TEST_RTPS_TRCH_MAILBOX
#define PLAT_RSE_AP_SND_MHU_BASE	UL(0x49000000)
#define PLAT_RSE_AP_RCV_MHU_BASE	UL(0x49100000)

#ifdef TEST_RTPS_TRCH_MAILBOX
int mbox_inits(const void *init_data)
{
	enum mhu_error_t err;
	const struct mhu_addr *mbx_addr = (const struct mhu_addr *)init_data;

	err = mhu_init_sender(mbx_addr->sender_base);
	if (err != MHU_ERR_NONE) {
		if (err == MHU_ERR_ALREADY_INIT) {
			INFO("[RSE-COMMS] Host to RSE MHU driver already initialized\n");
		} else {
			ERROR("[RSE-COMMS] Host to RSE MHU driver initialization failed: %d\n",
			      err);
			return -1;
		}
	}

	err = mhu_init_receiver(mbx_addr->receiver_base);
	if (err != MHU_ERR_NONE) {
		if (err == MHU_ERR_ALREADY_INIT) {
			INFO("[RSE-COMMS] RSE to Host MHU driver already initialized\n");
		} else {
			ERROR("[RSE-COMMS] RSE to Host MHU driver initialization failed: %d\n",
			      err);
			return -1;
		}
	}

	return 0;
}

static int mhu_init(void)
{
	struct mhu_addr mhu_addresses;

	/* Get sender and receiver frames for AP-RSE communication */
	mhu_addresses.sender_base = PLAT_RSE_AP_SND_MHU_BASE;
	mhu_addresses.receiver_base = PLAT_RSE_AP_RCV_MHU_BASE;

	/* Initialize the communication channel between AP and RSE */
	return mbox_inits(&mhu_addresses);
}
#endif

#if configUSE_IDLE_HOOK
void vApplicationIdleHook(void)
{
  asm volatile ("cpsid if \n"
                "dsb      \n"
                "isb      \n"
                "wfi      \n"
                "cpsie if \n");
}
#endif

static void hvc_putc(int c)
{
  asm volatile (
    "mov r1, %1\n"
    "mov r0, %0\n"
    "hvc #1 \n"
    : : "i"(0x86000000), "r"(c) : "r0", "r1");
}

void hvc_puts(char const *s)
{
  char c;
  while ((c = *s++))
    hvc_putc(c);
}


static void task1(void *param)
{
	(void)param;
	while(1) {
		hvc_puts("task1\n");
		cmd_recv();
		vTaskDelay(1000);
	}
}

static void task2(void *param)
{
	(void)param;
	while(1) {
		hvc_puts("task2\n");
		cmd_proc();
		vTaskDelay(2000);
	}
}

int main(void) {
  uart_init();
  vcx_vcmd_init(NULL);
  printf("R52 is alive\r\n");
  
  //rd/wr register
  {
	  uint8_t val=0xff;
	  val = readb(TIMER_BASE_ADDR);
	  writeb(val, TIMER_BASE_ADDR);
  }

  gic_setup(MAILBOX_IRQ);

#ifdef TEST_MAILBOX
  mhu_init();
#endif

  xTaskCreate(task1, "task1", 200, NULL, 2, NULL);
  xTaskCreate(task2, "task2", 200, NULL, 1, NULL);

  hvc_puts("Hello from FreeRTOS!\n");
  vTaskStartScheduler();

  while(1);

  vcx_vcmd_exit(NULL);
  return 0;
}
