#include "tb.h"

void PrintNeighbors(Neighbors* data) {
	printf("Neighbor Data:\n"
		   "  %d  \n"
		   "  |  \n"
		   "%d-%d-%d\n"
		   "  |  \n"
		   "  %d  \n\n", data->neighbors[NORTH],
						 data->neighbors[WEST],			
						 data->id,
						 data->neighbors[EAST],
						 data->neighbors[SOUTH]);
}

void PrintDisplay(Display* data) {
	char connections[NUM_NEIGHBORS] = {'X', 'X', 'X', 'X'};

	for (int i = 0; i < NUM_NEIGHBORS; i++) {
		if (data->neighborDisps[i] != NULL) {
			if (data->neighborDisps[i]->neighborDisps[NUM_NEIGHBORS - i - 1] == data) {
				connections[i] = (i == NORTH || i == SOUTH) ? '|' : '-';
			}
			else {
				connections[i] = 'O';
			}
		}
	}

	printf("Display ID: %d\n"
		   "   %c   \n"
		   "   %d   \n"
		   "  +-+  \n"
		   "%c%d| |%d%c\n"
		   "  +-+  \n"
		   "   %d   \n"
		   "   %c   \n\n", data->id,
						   connections[NORTH],
						   data->neighborIds[NORTH],
						   connections[WEST],
						   data->neighborIds[WEST],
						   data->neighborIds[EAST],
						   connections[EAST],
						   data->neighborIds[SOUTH],
						   connections[SOUTH]);
}

void PrintDisplayList(DisplayList* list) {
	
	DisplayNode* curr = NULL;

	printf("Display List (length: %d):\n", list->len);

	curr = list->root;
	while (curr != NULL) {
		PrintDisplay(curr->disp);
		curr = curr->next;
	}
}

void PrintArray(char* data, int len) {
	for (int i = 0; i < len; i++) {
		printf("%02x ", data[i]);
	}
	printf("\n");
}

void PrintDisplayGrid(DisplayGrid* grid) {
	printf("Grid (width: %d, height: %d):\n", grid->width, grid->height);
	for (int y = 0; y < grid->height; y++) {
		for (int x = 0; x < grid->width; x++) {
			printf("%d ", grid->dispIds[x][y]);
		}
		printf("\n\n");
	}
}

void PrintPixels(char* pixels) {
	printf("Pixel Data: \n");
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			printf("%d", (pixels[i] >> j) & 1);	
		}
		printf("\n");
	}
}

void PrintDisplayPixels(DisplayPixels* disp) {
	printf("display Pixel Data (width: %d, height %d): ", disp->modulesWidth, disp->modulesHeight);
}

void PrintImage(Image* image) {
	printf("Image Data: \n");
	for (int x = 0; x < image->width; x++) {
		for (int y = 0; y < image->height; y++) {
			printf("%02x ", image->bytes[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}

void TestSPITransfer(void) {

	char buf[8] = {61, 62, 63, 64, 65, 66, 67, 68};
	char* toRead = NULL;

	if (InitSPI() == 0) {
		return;
	}

	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_transfern(buf, 8);

	PrintArray(buf, 8);

	DeinitSPI();
}

void TestSPISend(void) {

	char toSend[8] = {10, 11, 12, 13, 14, 15, 16, 17};

	if (InitSPI() == 0) {
		return;
	}

	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	SendFrame(1, 8, toSend, 8);
	
	DeinitSPI();
}

void TestSPIRead(void) {

	char* toRead = NULL;
	int len = 8;

	if (InitSPI() == 0) {
		return;
	}

	toRead = ReadResponse(len);
	if (toRead == NULL) {
		printf("ERROR: Could not read response!\n");
		return;
	}

	PrintArray(toRead, len);

	free(toRead);
	
	DeinitSPI();
}

void TestCreateNeighbors(void) {

	Neighbors input;

	input.id = 1;
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 2;
	input.neighbors[WEST] = 4;
	input.neighbors[SOUTH] = 7;

	PrintNeighbors(&input);
}

void TestCreateDisplay(void) {

	Neighbors input;
	Display* output;

	input.id = 1;
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 2;
	input.neighbors[WEST] = 0;
	input.neighbors[SOUTH] = 0;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);

	PrintDisplay(output);
}

void TestCreateDisplayList(void) {

	Neighbors input;
	Display* output;
	DisplayList* list;

	list = CreateList();
	PrintDisplayList(list);

	input.id = 1;
	input.neighbors[NORTH] = 3;
	input.neighbors[EAST] = 2;
	input.neighbors[WEST] = 0;
	input.neighbors[SOUTH] = 0;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	input.id = 2;
	input.neighbors[NORTH] = 4;
	input.neighbors[EAST] = 0;
	input.neighbors[WEST] = 1;
	input.neighbors[SOUTH] = 0;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	input.id = 3;
	input.neighbors[NORTH] = 5;
	input.neighbors[EAST] = 4;
	input.neighbors[WEST] = 0;
	input.neighbors[SOUTH] = 1;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	input.id = 4;
	input.neighbors[NORTH] = 6;
	input.neighbors[EAST] = 0;
	input.neighbors[WEST] = 3;
	input.neighbors[SOUTH] = 2;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	input.id = 5;
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 6;
	input.neighbors[WEST] = 0;
	input.neighbors[SOUTH] = 3;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	input.id = 6;
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 0;
	input.neighbors[WEST] = 5;
	input.neighbors[SOUTH] = 4;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	FreeList(list);
}

void TestCreateDisplayGrid(int w, int h, int printNeighbors) {

	int id = 1;

	Neighbors input;
	Display* output;
	DisplayList* list;
	DisplayGrid* screen;

	list = CreateList();

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			input.id = id;
			input.neighbors[NORTH] = (id - w > 0) && (h != 1)? id - w : 0;
			input.neighbors[EAST] = (id % w != 0) && (w != 1) ? id + 1 : 0;
			input.neighbors[WEST] = (id % w != 1) && (w != 1) ? id - 1 : 0;
			input.neighbors[SOUTH] = (id + w <= w * h) && (h != 1) ? id + w : 0;

			if (printNeighbors) {
				PrintNeighbors(&input);
			}

			output = CreateDisplay(&input);
			InsertDisplay(list, output);

			id++;
		}
	}

	screen = CreateDisplayGrid(list);
	PrintDisplayGrid(screen);

	FreeList(list);
	FreeDisplayGrid(screen);
}

void TestAutoConfigure() {
	
	Neighbors* input = NULL;
	Display* disp = NULL;
	DisplayList* list = NULL;
	DisplayGrid* screen = NULL;
	
	InitSPI();

	list = CreateList();

	input = FindNeighborData(BCM2835_SPI_CS0);
	PrintNeighbors(input);

	InsertDisplay(list, CreateDisplay(input));
	free(input);

	input = FindNeighborData(BCM2835_SPI_CS1);
	PrintNeighbors(input);

	InsertDisplay(list, CreateDisplay(input));
	free(input);

	screen = CreateDisplayGrid(list);
	FreeList(list);

	PrintDisplayGrid(screen);
	FreeDisplayGrid(screen);
	
	DeinitSPI();
}

void TestConvertImage(char *filename) {

	Image* image = NULL;

	image = ReadPNGFile(filename);
	PrintImage(image);
	FreeImage(image);
}

void TestFPS(int printImage) {

	Image* image = NULL;
	char filename[20];

	time_t start = clock();
	for (int i = 1; i <= 150; i++) {
		snprintf(filename, 20, "./tmp/frame%d.png", i);
		image = ReadPNGFile(filename);
		if (printImage) {
			PrintImage(image);
		}
		FreeImage(image);
	}
	time_t stop = clock();

	printf("Result: %d images read in %lf seconds.\n", 150, (double) (stop - start) / CLOCKS_PER_SEC);
}

void TestUpdatePixels() {
	
	char buf[7] = {0};
	char tmp;
	int cnt;

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			buf[i] <<= 1;
			buf[i] |= (i + j) % 2;
		}
	}

	InitSPI();

	cnt = 0;
	while(cnt < 1) {
		SendPixelData(BCM2835_SPI_CS0, buf, 7);
		PrintPixels(buf);
		tmp = buf[0];
		memmove(buf, buf + 1, 6);
		buf[6] = tmp;
		sleep(1);
		cnt++;
	}

	DeinitSPI();
}
