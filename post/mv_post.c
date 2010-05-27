#include <common.h>

#include <post.h>
#include <i2c.h>

/* At this point, the code just suuports Marvell platform */
#ifdef CONFIG_MARVELL

#include "mvOs.h"
#include "../diag/diag.h"

typedef int (post_func_t) (void);

post_func_t *post_sequence[] = 
{
	mem_data_bus_test,      /* DRAM Data bus test */ 
	mem_address_bus_test,   /* DRAM Address bus test */
	uart_int_lpbk_test,     /* UART internal loopback test */
	nand_detect_test,       /* NAND detection test */
	nand_badblock_test,     /* NAND bad block detection test */
	rtc_test,               /* RTC test */
	NULL,
};

extern unsigned int _TEXT_BASE;

int mv_post_entry(int flags)
{
	int failure = 0, total, reg;
	post_func_t **post_func_ptr;

	printf("\nChecking for BootROM Routine Errors\n");

	reg = MV_REG_READ(0x100D0);

	printf("\nNo. of BootROM routine retries: %d\n", (reg & 0xff0000) >> 16);

	switch(reg & 0xff)
	{   
		case 0x32:
			printf("NAND: Nand image not aligned to 512 bytes\n");
			break;
		case 0x33:
			printf("NAND: Nand ECC error\n");
			break;
		case 0x34:
			printf("NAND: Nand timeout\n");
			break;
		case 0x35:
			printf("NAND: Nand excessive bad blocks\n");
			break;
		case 0x36:
			printf("NAND: Nand bad block encountered\n");
			break;
		case 0x00:
			printf("No error found\n");
			break;
		default:
			printf("Non-NAND error\n");
	}   

	/* Get the start and the end memory address offset for memory test */
	diag_get_mem_detail(&mem_test_start_offset, &mem_test_end_offset);

	printf("\nRunning POST...\n");

	for (post_func_ptr = post_sequence; *post_func_ptr; ++post_func_ptr)
	{
		printf("\n");
		failure += (*post_func_ptr)();
	}

	total = post_func_ptr - post_sequence;

	printf("\n%d/%d tests PASSED\n", total - failure, total);
	printf("POST completed\n");

	return 0;
}

#endif
