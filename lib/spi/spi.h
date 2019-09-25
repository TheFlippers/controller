#ifndef __SPI_H__
#define __SPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

typedef struct {
	uint8_t north;
	uint8_t east;
	uint8_t south;
	uint8_t west;
} Neighbors;

int InitSPI();
void DeinitSPI();
int SendGet();
int SendPost();
Neighbors* GetNeighborData(uint8_t channel);

#endif // __SPI_H__
