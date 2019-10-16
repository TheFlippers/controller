#define DEBUG

#include <dirent.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#include "tb.h"
#endif // DEBUG

#include "spi/spi.h"
#include "img/display.h"
#include "img/read_png.h"

int main(int argc, char* argv[]) {

	#ifdef DEBUG
	TestSPITransfer();
	//TestSPISend();
	//TestSPIRead();
	//TestCreateDisplay();
	//TestCreateDisplayList();
	//TestCreateDisplayGrid(2, 4);
	//TestAutoConfigure();
	//TestUpdatePixels();

	/*
	Image* image = NULL;
	char filename[20];

	for (int i = 1; i < 11; i++) {
		snprintf(filename, 20, "./tmp/frame%d.png", i);
		image = ReadPNGFile(filename);
		PrintImage(image);
		FreeImage(image);
	}
	*/

	#endif // DEBUG

    return EXIT_SUCCESS;
}
