#include <stdint.h>
#include "../include/reset.h"

void _reset()
{
	uint8_t* sbss = (uint8_t*)&_sbss;
	uint8_t* ebss = (uint8_t*)&_ebss;

	uint8_t* sdata = (uint8_t*)&_sdata;
	uint8_t* edata = (uint8_t*)&_edata;
	uint8_t* ldata = (uint8_t*)&_ldata;

	for(int i = 0; sbss + i < ebss; i++)
	{
		sbss[i] = 0;
	}
	

	for(uint32_t i = 0; sdata + i < edata; i++)
	{
		sdata[i] = ldata[i];
	}


	main();
}











