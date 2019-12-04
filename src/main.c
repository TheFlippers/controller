#include <dirent.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "img/display.h"
#include "img/read_png.h"
#include "spi/spi.h"
#include "text/read_txt.h"
#include "fdd.h"
#include "tb.h"

int main(int argc, char* argv[]) {
	
	// Initialize SPI drivers
	InitSPI();

	// Testcases
	/*
	int channel = atoi(argv[1]);
	SendDisplayID(channel, 69);
	FindNeighborData(channel, 10);
	*/

	//TestSPITransfer(0);
	//TestSPITransfer(atoi(argv[1]), atoi(argv[2]));

	// Operate flip disk display
	RunFDD();

	// Deinitialize SPI drivers
	DeinitSPI();


    return EXIT_SUCCESS;
}
