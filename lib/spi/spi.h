#ifndef __SPI_H__
#define __SPI_H__

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint8_t north;
	uint8_t east;
	uint8_t south;
	uint8_t west;
} Neighbors;

int InitSPI();
void DeinitSPI();
int SendFrame(int gpBit, uint8_t pid, char *data, size_t len);
char* ReadResponse(int len);
void PostPixelData(char* pixels);
Neighbors* GetNeighborData(uint8_t channel);

#endif // __SPI_H__
