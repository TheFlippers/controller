#include "tb.h"

void PrintNeighbors(Neighbors* data) {
	printf("Neihgbor Data:\n"
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
		printf("%02x\n", data[i]);
	}
}

void TestSPITransfer(void) {

	char buf[8] = {65, 66, 67, 68, 69, 70, 71, 72};
	char* toRead = NULL;

	if (InitSPI() == 0) {
		return;
	}

	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_transfern(buf, 6);

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
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 2;
	input.neighbors[WEST] = 0;
	input.neighbors[SOUTH] = 0;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	input.id = 2;
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 0;
	input.neighbors[WEST] = 1;
	input.neighbors[SOUTH] = 0;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);
	PrintDisplay(output);

	InsertDisplay(list, output);
	PrintDisplayList(list);

	FreeList(list);
}
