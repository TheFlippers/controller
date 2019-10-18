#define DEBUG

#include <dirent.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "img/display.h"
#include "img/read_png.h"
#include "spi/spi.h"
#include "tb.h"

int main(int argc, char* argv[]) {

	#ifdef TEST1
		TestSPITransfer();
	#endif // TEST1

	//TestSPISend();
	//TestSPIRead();

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

    return EXIT_SUCCESS;
}
