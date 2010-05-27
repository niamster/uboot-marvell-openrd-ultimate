#include <common.h>

#ifdef CONFIG_MARVELL

#include <diag.h>
#include "mvOs.h"

diag_func_t *diag_sequence[] = 
{
	mem_data_bus_test,      /* DRAM Data bus test */ 
	mem_address_bus_test,   /* DRAM Address bus test */
	mem_device_test,        /* DRAM device test */
	uart_int_lpbk_test,     /* UART internal loopback test */
#ifdef OPENRD_88F6281A
	uart_ext_lpbk_test,     /* UART external loopback test */
#endif
#ifdef MV_NAND
	nand_detect_test,       /* NAND detection test */
	nand_badblock_test,     /* NAND bad block detection test */
	nand_readwrite_test,    /* NAND read/write test - NOTE: this is distructive test */
#endif
	gbe_link_detect_test,	/* GbE link detection test */
	rtc_test,               /* RTC test */
	NULL,
};

char diag_cmd[][15] = 
{
	"data_bus",
	"addr_bus",
	"device",
	"uart_int",
#ifdef OPENRD_88F6281A
	"uart_ext",
#endif
#ifdef MV_NAND
	"nand_detect",
	"nand_bad_block",
	"nand_rw",
#endif
	"gbe_link",
	"rtc",
	"\0"
};

unsigned int *mem_test_start_offset;
unsigned int *mem_test_end_offset;

void run_diag(void)
{
	diag_func_t **diag_func_ptr;
	unsigned int reg = 0;
	
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

	printf("\n\nRunning diagnostics ...\n");

	for (diag_func_ptr = diag_sequence; *diag_func_ptr; ++diag_func_ptr)
	{
		if(*diag_func_ptr == &nand_readwrite_test)
		{
			continue;
		}
		
		printf("\n");

		if((*diag_func_ptr)())
			break;
	}

	if(*diag_func_ptr == NULL)
		printf("\nDiag completed\n");
	else
		printf("\nDiag FAILED\n");
}

void diag_init(char *board_name)
{
	/* Init interrupts - we would need this as we use timer interrupt in udelay */
	interrupt_init();

	/* Initialize board so that all the GPIO are set & we are good to init serial */
	diag_board_init(board_name);

	/* Get the start and the end memory address offset for memory test */
	diag_get_mem_detail(&mem_test_start_offset, &mem_test_end_offset);

	/* Serial init */
	diag_serial_init(DIAG_SERIAL_CONSOLE_PORT, DIAG_BAUD_RATE);
}

#else

/* Not supported for non-Marvell platforms */
void run_diag(void)
{
}

#endif
