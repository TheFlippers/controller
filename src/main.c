#define DEBUG

#include <stdio.h>

#ifdef DEBUG
#include "tb.h"
#endif // DEBUG

#include "spi/spi.h"
#include "img/display.h"

int main(int argc, char* argv[]) {

	#ifdef DEBUG
	//TestSPITransfer();
	//TestSPISend();
	//TestSPIRead();
	//TestCreateDisplay();
	//TestCreateDisplayList();
	TestCreateDisplayGrid(2, 4);
	#endif // DEBUG

    return EXIT_SUCCESS;
}
