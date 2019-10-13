#define DEBUG

#include <stdio.h>
#include <png.h>

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

	/*char header[8];
	FILE* fp = fopen("./tmp/frame0003.png", "rb");
	if (!fp) {
		return EXIT_SUCCESS;
	}

	fread(header, 1, 8, fp);
	if (!png_sig_cmp(header, 0, 8)) {
		printf("is png\n");
	}
	else {
		printf("not png\n");
	}
*/
	#endif // DEBUG

    return EXIT_SUCCESS;
}
