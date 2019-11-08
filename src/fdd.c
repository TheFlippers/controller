#include "fdd.h"

void ConfigureFDD(char* filename) {

	uint8_t channels[2] = {BCM2835_SPI_CS0, BCM2835_SPI_CS1};
	Neighbors* input = NULL;
	Display* disp = NULL;
	DisplayList* list = NULL;
	DisplayGrid* screen = NULL;

	// Create struct for saving neighbor data
	list = CreateList();

	// Assign ID values to displays
	for (int i = 0; i < 2; i++) {
		SendDisplayID(channels[i], i + 1);
	}

	// Accquire neighbor information from displays
	for (int i = 0; i < 2; i++) {
		input = FindNeighborData(channels[i]);
		InsertDisplay(list, CreateDisplay(input));
		free(input);
	}

	// Calculate screen size
	screen = CreateDisplayGrid(list);
	FreeList(list);

	// Save screen information to configuration file
	SaveDisplayGrid(filename, screen);

	// Free memory
	FreeDisplayGrid(screen);
}

void DisplayFrames(char* filename, DisplayGrid* grid) {

	FILE* fp = NULL;
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;
	int num_frames = 0;
	char frame[25] = "";
	Image* image = NULL;
	uint8_t channels[2] = {BCM2835_SPI_CS0, BCM2835_SPI_CS1};
	char* buf = NULL;

	// Open file for reading
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Could not open file for reading!\n");
		return;
	}

	// Read file to determine number of frames
	while ((read = getline(&line, &len, fp)) != -1) {
		num_frames = atoi(line);
	}
	if (line != NULL) {
		free(line);
	}

	// Close frame count file
	fclose(fp);

	// Display frames
	for (int i = 1; i < num_frames; i++) {

		// Read frame into image
		snprintf(frame, 25, FRAME_FILENAME, i);
		image = ReadPNGFile(frame);

		// Convert image to pixels
		for (int y = 0; y < grid->height; y++) {
			for (int x = 0; x < grid->width; x++) {
					buf = ImageToDisplayPixels(image, grid->width, grid->height, x, y);
					SendPixelData(channels[grid->dispIds[x][y] - 1], buf, 7);
					free(buf);
			}
		}

		// Wait for next frame
		FreeImage(image);
		usleep(FRAME_PERIOD);
	}
}

void DisplayText(char* filename, DisplayGrid* grid) {

	char* text = NULL;
	Message msg;
	int charIndex = 0;
	int charOffset = 0;
	char buf[7] = {0};

	// Ensure valid display height
	if (grid->height != 1) {
		fprintf(stderr, "ERROR: Invalid display configuration!\n");
		return;
	}

	// Read text from file
	text = ReadTXTFile(filename);

	// Convert text to pixels
	TextToPixels(text, &msg);

	// Scroll through letters to display pixels
	for (int i = 0; i < msg.len * CHAR_WIDTH; i++) {

		// Calculate current position in message
		charIndex = i / CHAR_WIDTH;
		charOffset = i % CHAR_WIDTH;

		// Find slice of message for each display
		for (int x = 0; x < grid->width; x++) {

			// Clear previous character
			memset(buf, 0, CHAR_HEIGHT);

			// Copy pixels into buffer row-wise
			if (charIndex + x < msg.len) {
				for (int j = 0; j < CHAR_HEIGHT; j++) {
					memcpy(buf + j, msg.letters[charIndex + x].pixels + j, 1);
					buf[j] <<= charOffset;
					if (charIndex + x < msg.len - 1) {
						buf[j] |= msg.letters[charIndex + x + 1].pixels[j] >> (CHAR_WIDTH - charOffset);
					}
				}
			}

			/* VertMirrorDisplay(buf); */

			// Transmit pixels
			SendPixelData(BCM2835_SPI_CS0, buf, CHAR_HEIGHT);
		}

		// Wait for next frame
		usleep(FRAME_PERIOD);
	}

	// Memory clean-up
	FreeMessage(&msg);
	free(text);
}

void MirrorDisplay(char* pixels) {

	char tmp = 0;

	// Mirror each row
	for (int y = 0; y < MODULE_SIZE; y++) {

		// Shift out empty parity bit
		pixels[y] >>= 1;

		// Shift into buffer to mirror data
		tmp = 0;
		for (int x = 0; x < MODULE_SIZE; x++) {
			tmp |= (pixels[y] >> x) & 1;
			tmp <<= 1;
		}

		// Save new value
		pixels[y] = tmp;
	}

}

void HorizMirrorDisplay(char* pixels) {

	char tmp = 0;

	// Loop through half of display and swap values
	for (int y = 0; y < MODULE_SIZE / 2; y++) {
		tmp = pixels[y];
		pixels[y] = pixels[MODULE_SIZE - y];
		pixels[MODULE_SIZE - y] = tmp;
	}

}

// TODO: finish rotation code
void RotateClockwise(char* pixels) {

	char tmp[7] = {0};

	// Perform transformation on temporary buffer
	for (int i = 0; i < MODULE_SIZE; i++) {
		for(int j = 0; j < MODULE_SIZE; j++) {

		}
	}


	// overwrite pixels
	memcpy(pixels, tmp, MODULE_SIZE);

}
