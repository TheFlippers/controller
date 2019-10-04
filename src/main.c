#define DEBUG

#include <stdio.h>

#ifdef DEBUG
#include "tb.h"
#endif // DEBUG

#include "spi/spi.h"
#include "img/display.h"

int main(int argc, char* argv[]) {

	#ifdef DEBUG
	TestCreateDisplay();
	#endif // DEBUG

/*
	char toSend[8] = {5, 6, 7, 8, 9, 10, 11, 12};

	if (InitSPI() == 0) {
		return EXIT_FAILURE;
	}


	FindNeighborData(BCM2835_SPI_CS0);


	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	//SendFrame(1, 8, toSend, 8);
	bcm2835_spi_transfern(toSend, 8);
	for (int i = 0; i < 8; i++) {
		printf("0x%02x\n", toSend[i]);
	}


	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
	SendFrame(1, 8, toSend, 8);


    DeinitSPI();
*/

    return EXIT_SUCCESS;
}
