#include "spi/spi.h"

int InitSPI() {

    if (!bcm2835_init()) {
	printf("ERROR: Failed to initialize BCM!\n");
	return 0;
    }

    if (!bcm2835_spi_begin()) {
	printf("ERROR: Failed to initialize SPI!\n");
	return 0;
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);

    return 1;
}

void DeinitSPI() {

    bcm2835_spi_end();
    bcm2835_close();
}

int SendGet() {
	return 0;
}
int SendPost() {
	return 0;
}

Neighbors* GetNeighborData(uint8_t channel) {

	char buf[4] = {1, 0, 0, 0};
	int len = 4;

	bcm2835_spi_chipSelect(channel);
	bcm2835_spi_transfern(buf, len);

	Neighbors* output = malloc(sizeof(*output));	

	output->north = buf[0];
	output->east = buf[1];
	output->south = buf[2];
	output->west = buf[3];

	return output;
}
