#ifndef __SPI_H__
#define __SPI_H__

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAME_LEN 8
#define HEADER_BITMASK 127
#define GP_BITMASK 1
#define GP_BITSHIFT 7
#define NEIGHBOR_FRAME_LEN 6

// TODO: implement method of saving ids for each channel

enum FrameType {
	POST = 0,
	GET = 1
};

enum GetPIDs {
	GET_NEIGHBOR_DATA = 0
};

enum PostPIDs {
	POST_NEIGHBOR_DATA = 0,
	POST_PIXEL_DATA = 1,
	POST_DISPLAY_ID = 2
};

enum Directions {
	NORTH = 0,
	EAST = 1,
	WEST = 2,
	SOUTH = 3
};

typedef struct {
	uint8_t id;
	uint8_t neighbors[4];
} Neighbors;

int InitSPI();
void DeinitSPI();
int SendFrame(int gpBit, uint8_t pid, char *data, size_t len);
char* ReadResponse();
void SendPixelData(uint8_t channel, char* pixels, size_t len);
void SendDisplayID(uint8_t channel, uint8_t id);
Neighbors* FindNeighborData(uint8_t channel);

#endif // __SPI_H__
