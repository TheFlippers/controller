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
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);
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

	// Create frame buffer
	buf = malloc(sizeof(*buf) * (len + 1));
	if (buf == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return 1;
	}

	// Create header from pid
	header = pid;

	// Add Get/Post bit to header
	header &= 254;
	header |= gpBit & 1;

	// buffer frame data
	buf[0] = header;
	memcpy((buf + 1), data, len);
	frameLen = len + 1;

	// Transmit frame
	bcm2835_spi_transfern(buf, frameLen);

	return 0;
}

char* ReadResponse(int len) {

	char *buf = NULL;

	// Create data buffer
	buf = malloc(sizeof(*buf) * len);
	if (buf == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return buf;
	}

	// Clear data buffer
	memset(buf, 0, len);

	// Transmit dummy bytes to initiate read
	bcm2835_spi_transfern(buf, len);

	return buf;
}

void PostPixelData(char* pixels) {

}

Neighbors* GetNeighborData(uint8_t channel) {
	return NULL;
}
