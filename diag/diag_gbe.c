#include <common.h>

#include "diag.h"
#include "eth-phy/mvEthPhy.h"
#include "mvBoardEnvLib.h"

#if defined(OPENRD_88F6281A) || defined(OPENRD_ULTIMATE_88F6281A)
extern MV_32 is_client;
#endif

/*******************************************************************************
*
* Name        : gbe_link_detect_test
* Description : GbE Link Detection Test
*
* Input arg   : None
* Output arg  : None
* Return      : Returns 1 on failure, else 0
*******************************************************************************/

int gbe_link_detect_test()
{
	unsigned short val = 0;
	unsigned int result = 0;

	/* read specific status reg */ 
	if( mvEthPhyRegRead( mvBoardPhyAddrGet(0), ETH_PHY_SPEC_STATUS_REG, &val) != MV_OK )
		return MV_ERROR;

	switch (val & ETH_PHY_SPEC_STATUS_SPEED_MASK)
	{   
		case ETH_PHY_SPEC_STATUS_SPEED_1000MBPS:
			printf( "\tSpeed: 1000 Mbps, " );
			break;
		case ETH_PHY_SPEC_STATUS_SPEED_100MBPS:
			printf( "\tSpeed: 100 Mbps, " );
			break;
		case ETH_PHY_SPEC_STATUS_SPEED_10MBPS:
			printf( "\tSpeed: 10 Mbps, " );
			break;
		default:
			printf( "\tSpeed: Uknown, " );
			break;
	}   

	if( val & ETH_PHY_SPEC_STATUS_DUPLEX_MASK ) 
	{
		printf( "Duplex: Full, " );
	}
	else 
	{
		printf( "Duplex: Half, " );
	}

	if( val & ETH_PHY_SPEC_STATUS_LINK_MASK )
	{
		printf("Link: up\n");
		result = 0;
	}
	else
	{
		printf("Link: down\n");
		result = 1;
	}
	printf("\tGbE0 link detect test                            ");
	printf(((result)? "FAILED\n":"PASSED\n"));

#if defined(OPENRD_88F6281A) || defined(OPENRD_ULTIMATE_88F6281A)
	if(MV_TRUE == is_client)
	{
		printf("\n");

		/* read specific status reg */ 
		if( mvEthPhyRegRead( mvBoardPhyAddrGet(1), ETH_PHY_SPEC_STATUS_REG, &val) != MV_OK )
			return MV_ERROR;

		switch (val & ETH_PHY_SPEC_STATUS_SPEED_MASK)
		{   
			case ETH_PHY_SPEC_STATUS_SPEED_1000MBPS:
				printf( "\tSpeed: 1000 Mbps, " );
				break;
			case ETH_PHY_SPEC_STATUS_SPEED_100MBPS:
				printf( "\tSpeed: 100 Mbps, " );
				break;
			case ETH_PHY_SPEC_STATUS_SPEED_10MBPS:
				printf( "\tSpeed: 10 Mbps, " );
				break;
			default:
				printf( "\tSpeed: Uknown, " );
				break;
		}   

		if( val & ETH_PHY_SPEC_STATUS_DUPLEX_MASK ) 
		{	
			printf( "Duplex: Full, " );
		}
		else 
		{
			printf( "Duplex: Half, " );
		}

		if( val & ETH_PHY_SPEC_STATUS_LINK_MASK )
		{	
			printf("Link: up\n");
			result = 0;
		}
		else
		{
			printf("Link: down\n");
			result = 1;
		}
		printf("\tGbE1 link detect test                            ");
		printf(((result)? "FAILED\n":"PASSED\n"));
	}
#endif
	return result;
}

