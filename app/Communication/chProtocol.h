#include "stm32f4xx.h"

#define CHPRO_MAX_DATA_LEN  50

typedef struct 
{
	uint8_t data_type;
	float param[CHPRO_MAX_DATA_LEN];
	uint8_t data_len;
} ch_data_pck;

