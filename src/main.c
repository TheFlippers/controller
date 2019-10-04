#include <stdio.h>

#include "spi/spi.h"

int main(int argc, char* argv[]) {

	char toSend[8] = {5, 6, 7, 8, 9, 10, 11, 12};

	if (InitSPI() == 0) {
		return EXIT_FAILURE;
	}

/*
	FindNeighborData(BCM2835_SPI_CS0);
*/

	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	SendFrame(1, 8, toSend, 8);

/*
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
	SendFrame(1, 8, toSend, 8);
*/

    DeinitSPI();

    return EXIT_SUCCESS;
}
