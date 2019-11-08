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
	
	InitSPI();

	// Run configurations to find display size
	if (argc >= 2 && !strcmp(argv[1], "config")) {

		// Configure display using SPI serial communication
		ConfigureFDD("fdd_conf");
	}
	// Run main execution loop
	else if (argc >= 2) {
		
		// Read grid from config file
		DisplayGrid* grid = NULL;
		grid = LoadDisplayGrid("fdd_conf");
		if (grid == NULL) {
			fprintf(stderr, "ERROR: Could not load display configuration!\n");
			DeinitSPI();
			return EXIT_FAILURE;
		}

		// Display images
		if (!strcmp(argv[1], "images")) {
			while (1) {
				DisplayFrames("./tmp/num_frames", grid);
			}
		}
		// Display text
		else if(!strcmp(argv[1], "text")) {
			while (1) {
				DisplayText("./tmp/message.txt", grid);
			}
		}

		// Memory clean-up
		FreeDisplayGrid(grid);
	}

	DeinitSPI();

    return EXIT_SUCCESS;
}
