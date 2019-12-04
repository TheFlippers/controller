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

void PrintMessage(Message* msg) {
	printf("Message Data (len %d): \n", msg->len);
	for (int i = 0; i < msg->len; i++) {
		printf("Character: %c (%d)\n", msg->letters[i].character, msg->letters[i].character);
		for (int j = 0; j < 7; j++) {
			for (int k = 7; k >= 0; k--) {
				printf("%d", (msg->letters[i].pixels[j] >> k) & 1);	
			}
			printf("\n");
		}
	}
}

void TestSPITransfer(int channel, int test) {

	char buf[4][8] = {{8, 7, 6, 5, 4, 3, 2, 1},
						{1, 2, 3, 4, 5, 6, 7, 8},
						{0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8},
						{0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55}};

	if (InitSPI() == 0) {
		return;
	}

	printf("TX: ");
	PrintArray(buf[test], 8);

	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_transfern(buf[test], 8);

	printf("RX: ");
	PrintArray(buf[test], 8);

	DeinitSPI();
}

void TestSPISend(void) {

	char toSend[7] = {10, 11, 12, 13, 14, 15, 16};

	if (InitSPI() == 0) {
		return;
	}

	printf("TX: ");
	PrintArray(toSend, 7);

	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	SendFrame(1, 2, toSend, 7);
	
	DeinitSPI();
}

void TestGetNeighbors(void) {

	if (InitSPI() == 0) {
		return;
	}

	SendFrame(GET, GET_NEIGHBOR_DATA, NULL, 0);

	DeinitSPI();
}

void TestSendDisplayID(void) {

	uint8_t id = 1;
	
	if (InitSPI() == 0) {
		return;
	}

	SendDisplayID(BCM2835_SPI_CS0, id);

	DeinitSPI();
}

void TestSendPixelData(void) {

	char buf[7] = {10, 12, 14, 16, 18, 20, 22};

	if (InitSPI() == 0) {
		return;
	}

	printf("TX: ");
	PrintArray(buf, 7);

	SendPixelData(BCM2835_SPI_CS0, buf, 7);

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

	SaveDisplayGrid("config.txt", screen);

	FreeList(list);
	FreeDisplayGrid(screen);
}

void TestAutoConfigure() {
	
	uint8_t channels[2] = {BCM2835_SPI_CS0, BCM2835_SPI_CS1};
	Neighbors* input = NULL;
	Display* disp = NULL;
	DisplayList* list = NULL;
	DisplayGrid* screen = NULL;
	
	InitSPI();

	list = CreateList();

	for (int i = 0; i < 2; i++) {
		SendDisplayID(channels[i], i + 1);
	}

	for (int i = 0; i < 2; i++) {
		input = FindNeighborData(channels[i], 10);
		PrintNeighbors(input);
		InsertDisplay(list, CreateDisplay(input));
		free(input);
	}

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
	
	char buf[7] = {0x1a, 0x00};
	int cnt;

	InitSPI();

	cnt = 0;
	while(cnt < 500) {
		SendPixelData(BCM2835_SPI_CS0, buf, 7);
		PrintPixels(buf);
		memmove(buf + 1, buf, 6);
		buf[0] = (cnt % 2 == 1) ? 1 << (rand() % 8) : buf[0];
		usleep(67000);
		cnt++;
	}

	DeinitSPI();
}

void TestReadMessage() {

	char* txt = NULL;
	Message msg;

	txt = ReadTXTFile("./tmp/message.txt");
	TextToPixels(txt, &msg);

	PrintMessage(&msg);
	
	FreeMessage(&msg);
	free(txt);
}

void RunTests() {

	#ifdef TEST1
		TestSPITransfer();
	#endif // TEST1

	#ifdef TESTA
		TestSPISend();
	#endif // TESTA
	
	#ifdef TESTB
		TestSPIRead();
	#endif // TESTB

	#ifdef TESTC
		TestSendDisplayID();
	#endif // TESTC

	#ifdef TESTD
		TestSendPixelData();
	#endif // TESTD

	#ifdef TESTE
		TestGetNeighbors();
	#endif // TESTE

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

	#ifdef TESTF
		TestAutoConfigure();
	#endif //TESTF

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

		clock_t start, end;

		// Begin execution time measurement
		start = clock();

		// Setup SPI communications
		InitSPI();

		// Read each image file
		for (int i = 1; i < 150; i++) {

			// Open image file
			snprintf(filename, 20, "./tmp/frame%d.png", i);
			image = ReadPNGFile(filename);

			// Convert PNG to pixel data
			buf = ImageToDisplayPixels(image, 1, 1, 0, 0);
			PrintPixels(buf);

			// Transmit pixel data
			SendPixelData(BCM2835_SPI_CS0, buf, 7);

			// Memory clean-up
			free(buf);
			FreeImage(image);
		}

		// Close SPI connections
		DeinitSPI();

		// Stop execution time measurement
		end = clock();

		// Display statistics
		printf("Execution time: %f (in clock cycles)\n", (double) end - start);
		printf("Execution time: %f (in clock seconds)\n", (double) (end - start) / CLOCKS_PER_SEC);
		printf("Frame rate: %f FPS (for 150 frame sample)\n", 150 * CLOCKS_PER_SEC / (double) (end - start));

	#endif // TEST9
}
