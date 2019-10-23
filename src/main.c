#define DEBUG

#include <dirent.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "img/display.h"
#include "img/read_png.h"
#include "spi/spi.h"
#include "tb.h"

int main(int argc, char* argv[]) {

	#ifdef TEST1
		TestSPITransfer();
	#endif // TEST1

	#ifdef TESTA
		TestSPISend();
	#endif
	
	#ifdef TESTB
		TestSPIRead();
	#endif

	#ifdef TESTC
		TestSendDisplayID();
	#endif

	#ifdef TESTD
		TestSendPixelData();
	#endif

	#ifdef TEST2
		TestCreateNeighbors();	
	#endif // TEST2

	//TestCreateDisplay();
	//TestCreateDisplayList();

	#ifdef TEST3
		TestCreateDisplayGrid(3, 1, 1);
	#endif // TEST3

	#ifdef TEST4
		TestCreateDisplayGrid(5, 3, 0);
	#endif // TEST4

	//TestAutoConfigure();

	#ifdef TEST5
		TestConvertImage("./tmp/frame3.png");
	#endif // TEST5

	#ifdef TEST6
		TestFPS(1);
	#endif // TEST6

	#ifdef TEST7
		TestFPS(0);
	#endif // TEST7

	#ifdef TEST8
		TestUpdatePixels();
	#endif // TEST8

	#ifdef TEST9
		Image* image = NULL;
		char filename[20] = {0};
		char* buf = NULL;

		InitSPI();

		for (int i = 1; i < 150; i++) {
			snprintf(filename, 20, "./tmp/frame%d.png", i);
			image = ReadPNGFile(filename);
			buf = ImageToDisplayPixels(image, 0, 0, 0, 0);
			PrintPixels(buf);
			SendPixelData(BCM2835_SPI_CS0, buf, 7);
			free(buf);
			FreeImage(image);
			usleep(67000);
		}

		DeinitSPI();

	#endif // TEST9

    return EXIT_SUCCESS;
}
