#include "spi/spi.h"

int InitSPI() {

	// Initialize BCM drivers
    if (!bcm2835_init()) {
	fprintf(stderr, "ERROR: Failed to initialize BCM!\n");
	return 0;
    }

	// Initialize BCM SPI drivers
    if (!bcm2835_spi_begin()) {
	fprintf(stderr, "ERROR: Failed to initialize SPI!\n");
	return 0;
    }

	// Set SPI parameters
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);

    return 1;
}

void DeinitSPI() {

	// Stop BCM drivers
    bcm2835_spi_end();
    bcm2835_close();
}

int SendFrame(int gpBit, uint8_t pid, char *data, size_t len) {
	
	char *buf = NULL;
	uint8_t header = 0;
	uint32_t frameLen = 0;

	// Ensure valid data length
	if (len > FRAME_LEN - 1 || len < 1) {
		fprintf(stderr, "ERROR: Invalid frame data length!\n");
		return 1;
	}

	// Create frame buffer
	buf = malloc(sizeof(*buf) * FRAME_LEN);
	if (buf == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return 1;
	}
	memset(buf, 0, FRAME_LEN);

	// Create header from pid
	header = pid;

	// Add Get/Post bit to header
	header &= HEADER_BITMASK;
	header |= (gpBit & GP_BITMASK) << GP_BITSHIFT;

	// buffer frame data
	buf[0] = header;
	memcpy((buf + 1), data, len);

	// Transmit frame
	bcm2835_spi_transfern(buf, FRAME_LEN);

	free(buf);

	return 0;
}

char* ReadResponse() {

	char *buf = NULL;

	// Create data buffer
	buf = malloc(sizeof(*buf) * FRAME_LEN);
	if (buf == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return buf;
	}

	// Clear data buffer
	memset(buf, 0, FRAME_LEN);

	// Transmit dummy bytes to initiate read
	bcm2835_spi_transfern(buf, FRAME_LEN);

	return buf;
}

void SendPixelData(uint8_t channel, char* pixels, size_t len) {

	
	// Select desired SPI slave channel
	bcm2835_spi_chipSelect(channel);

	// Send frame with header information
	SendFrame(POST, POST_PIXEL_DATA, pixels, len);
}

Neighbors* FindNeighborData(uint8_t channel) {

	char* buf = NULL;
	Neighbors* out = NULL;

	// Create neigbor data structure
	out = malloc(sizeof(*out));
	if (out == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return out;
	}
	// Select desired SPI slave channel
	bcm2835_spi_chipSelect(channel);

	// Send get request to slave
	SendFrame(GET, GET_NEIGHBOR_DATA, buf, 0);

	// Read neighbor data from device
	buf = ReadResponse(NEIGHBOR_FRAME_LEN);

	// Copy data to output structure
	out->id = buf[1];
	out->neighbors[NORTH] = buf[2];
	out->neighbors[EAST] = buf[3];
	out->neighbors[WEST] = buf[4];
	out->neighbors[SOUTH] = buf[5];
	free(buf);

	return out;
}
