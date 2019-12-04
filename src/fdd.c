#include "fdd.h"

uint8_t channels[2] = {BCM2835_SPI_CS0, BCM2835_SPI_CS1};

int killFlag = 0;
pthread_mutex_t killLock;

char inputBuffer[MAX_READ] = "";
pthread_t inputThread;
pthread_mutex_t inputLock;
int inputFlag = 0;

pthread_t displayThread;
pthread_mutex_t interruptLock;
int interruptFlag = 0;

void RunFDD(void) {

	int run = 1;
	DisplayGrid* screen = NULL;
	char buf[MAX_READ] = "";
	char* token = NULL;
	int cmdLen = 0;
	char* cmdData = NULL;
	Arguments args;

	// Configure display
	// TODO: fix configuration
	//screen = ConfigureFDD();
	screen = LoadDisplayGrid(CONFIG_FILENAME);

	// Create input thread
	pthread_create(&inputThread, NULL, ReadFromFIFO, NULL);

	// Run control loop
	while (run) {

		// Check input flag for data to read
		pthread_mutex_lock(&inputLock);
		if (inputFlag) {
			printf("received input\n");
			strncpy(buf, inputBuffer, MAX_READ);
			memset(inputBuffer, 0, MAX_READ);
			inputFlag = 0;
		}
		pthread_mutex_unlock(&inputLock);

		// Update display state when new state received
		if (strcmp(buf, "")) {
			printf("input: %s\n", buf);

			// Stop current mode thread
			pthread_mutex_lock(&interruptLock);
			interruptFlag = 1;
			pthread_mutex_unlock(&interruptLock);

			printf("interrupt set\n");
			// Wait for current thread to exit
			pthread_join(displayThread, NULL);
			pthread_mutex_lock(&interruptLock);
			interruptFlag = 0;
			pthread_mutex_unlock(&interruptLock);
			printf("threads joined\n");

			// Read new mode
			token = strtok(buf, " ");

			// Run different modes
			if (!strcmp(token, "images")) {
				printf("Displaying images!\n");

				// spawn display thread for images
				pthread_create(&displayThread, NULL, DisplayImages, screen);
			}
			else if (!strcmp(token, "text")) {
				printf("Displaying text!\n");

				// spawn display thread for text
				pthread_create(&displayThread, NULL, DisplayText, screen);
			}
			else if (!strcmp(token, "frame")) {
				printf("Displaying frame!\n");

				// Copy frame string into data
				cmdLen = strlen(buf + strlen(token) + 1);
				cmdData = malloc(sizeof(*cmdData) * (cmdLen + 1));
				strncpy(cmdData, buf + strlen(token) + 1, cmdLen);
				cmdLen = 0;

				// Create arguments structure
				args.grid = screen;
				args.data = cmdData;

				printf("frame data: %s\n", args.data);

				// Spawn display thread for individual frame
				pthread_create(&displayThread, NULL, DisplayFrame, &args);
			}
			else if (!strcmp(token, "exit")) {
				break;
			}

			// Clear read buffer
			memset(buf, 0, MAX_READ);
		}

		// Wait one second for next check
		sleep(1);
	}

	// Close existing threads
	pthread_mutex_lock(&killLock);
	killFlag = 1;
	pthread_mutex_unlock(&killLock);

	// Wait for thread termination
	pthread_join(inputThread, NULL);
	pthread_join(displayThread, NULL);

	// Clear all flags
	pthread_mutex_lock(&killLock);
	pthread_mutex_lock(&interruptLock);
	killFlag = 0;
	interruptFlag = 0;
	pthread_mutex_unlock(&interruptLock);
	pthread_mutex_unlock(&killLock);
}

void* ReadFromFIFO(void* args) {

	char buf[MAX_READ] = "";
	int fd = 0;

	// Create fifo
	umask(0);
	mkfifo(FIFO_PATH, 0666);
	fd = open(FIFO_PATH, O_RDONLY);

	// Continually read input from fifo
	while (1) {

		// Check kill status
		pthread_mutex_lock(&killLock);
		if (killFlag) {
			pthread_mutex_unlock(&killLock);
			break;
		}
		pthread_mutex_unlock(&killLock);

		// Read fifo until data is received
		ssize_t res = read(fd, buf, MAX_READ);
		if (res <= 0) {
			continue;
		}

		// Update buffer and raise input flag
		pthread_mutex_lock(&inputLock);
		strncpy(inputBuffer, buf, MAX_READ);
		memset(buf, 0, MAX_READ);
		inputFlag = 1;
		pthread_mutex_unlock(&inputLock);
	}
	printf("closing\n");

	// Close fifo
	close(fd);
	remove(FIFO_PATH);
}

DisplayGrid* ConfigureFDD(void) {

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

	// Wait for displays to detect neighbors
	sleep(CONFIG_DELAY);

	// Accquire neighbor information from displays
	for (int i = 0; i < 2; i++) {
		input = FindNeighborData(channels[i], 1000);
		if (input == NULL) {
			fprintf(stderr, "ERROR: Could not configure display!\n");
			FreeList(list);
			return NULL;
		}
		PrintNeighbors(input);
		if (input->id != 0) {
			InsertDisplay(list, CreateDisplay(input));
		}
		free(input);
	}

	// Calculate screen size
	screen = CreateDisplayGrid(list);
	FreeList(list);

	return screen;
}

void* DisplayImages(void* args) {

	DisplayGrid* grid = NULL;
	FILE* fp = NULL;
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;
	int num_frames = 0;
	char frame[25] = "";
	Image* image = NULL;
	char* buf = NULL;

	// Read arguments
	grid = (DisplayGrid*) args;

	// Open file for reading
	fp = fopen(COUNT_FILENAME, "r");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Could not open file for reading!\n");
		return NULL;
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

	printf("number of frames: %d\n", num_frames);
	// Continually show images
	while (1) {
	
		// Check for interrupt
		pthread_mutex_lock(&killLock);
		pthread_mutex_lock(&interruptLock);
		if (interruptFlag || killFlag) {
			printf("interrupt found (outer)\n");
			pthread_mutex_unlock(&interruptLock);
			pthread_mutex_unlock(&killLock);
			break;	
		}
		pthread_mutex_unlock(&interruptLock);
		pthread_mutex_unlock(&killLock);

		for (int i = 1; i < num_frames; i++) {

			// Check for interrupt
			pthread_mutex_lock(&killLock);
			pthread_mutex_lock(&interruptLock);
			if (interruptFlag || killFlag) {
				printf("interrupt found (inner)\n");
				pthread_mutex_unlock(&interruptLock);
				pthread_mutex_unlock(&killLock);
				break;	
			}
			pthread_mutex_unlock(&interruptLock);
			pthread_mutex_unlock(&killLock);

			// Read frame into image
			printf("Reading frame %d\n", i);
			snprintf(frame, 25, FRAME_FILENAME, i);
			image = ReadPNGFile(frame);

			// Convert image to pixels
			for (int y = 0; y < grid->height; y++) {
				for (int x = 0; x < grid->width; x++) {
						buf = ImageToDisplayPixels(image, grid->width, grid->height, x, y);
						PrintPixels(buf);
						SendPixelData(channels[grid->dispIds[x][y] - 1], buf, MODULE_SIZE);
						free(buf);
				}
			}

			// Wait for next frame
			FreeImage(image);
			usleep(FRAME_PERIOD);
		}
	}

	printf("exiting images\n");

	return NULL;
}

void* DisplayText(void* args) {

	DisplayGrid* grid = NULL;
	char* text = NULL;
	Message msg;
	int charIndex = 0;
	int charOffset = 0;
	char buf[7] = {0};

	// Read arguments
	grid = (DisplayGrid*) args;

	// Ensure valid display height
	if (grid->height != 1) {
		fprintf(stderr, "ERROR: Invalid display configuration!\n");
		return NULL;
	}

	// Read text from file
	text = ReadTXTFile(TEXT_FILENAME);

	// Convert text to pixels
	TextToPixels(text, &msg);
	free(text);

	// Continuously display message
	while (1) {

		// Check for interrupt
		pthread_mutex_lock(&killLock);
		pthread_mutex_lock(&interruptLock);
		if (interruptFlag || killFlag) {
			pthread_mutex_unlock(&interruptLock);
			pthread_mutex_unlock(&killLock);
			break;	
		}
		pthread_mutex_unlock(&interruptLock);
		pthread_mutex_unlock(&killLock);

		// Scroll through letters to display pixels
		for (int i = 0; i < msg.len * CHAR_WIDTH; i++) {

			// Check for interrupt
			pthread_mutex_lock(&killLock);
			pthread_mutex_lock(&interruptLock);
			if (interruptFlag || killFlag) {
				pthread_mutex_unlock(&interruptLock);
				pthread_mutex_unlock(&killLock);
				break;	
			}
			pthread_mutex_unlock(&interruptLock);
			pthread_mutex_unlock(&killLock);

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
				PrintPixels(buf);
				SendPixelData(channels[grid->dispIds[x][0] - 1], buf, MODULE_SIZE);
			}

			// Wait for next frame
			usleep(4 * FRAME_PERIOD);
		}
	}

	// Memory clean-up
	FreeMessage(&msg);

	return NULL;
}

void* DisplayFrame(void* args) {

	int x = 0;
	int y = 0;
	DisplayGrid* grid = NULL;
	char* input = NULL;
	char* token = NULL;
	char buf[7] = {0};
	
	// Read arguments
	grid = ((Arguments*) args)->grid;
	input = ((Arguments*) args)->data;

	printf("Reading frame\n");

	// Separate input string for parsing
	token = strtok(input, " ");
	while (token != NULL) {
		
		// Scan for display coordinates
		sscanf(token, "(%d,%d)", &x, &y);

		// Read bytes into buffer row-wise
		for (int i = 0; i < MODULE_SIZE; i++) {
			token = strtok(NULL, " ");
			sscanf(token, "%hx", buf + i);
		}

		// Transmit pixels
		PrintPixels(buf);
		SendPixelData(channels[grid->dispIds[x][y] - 1], buf, MODULE_SIZE);

		// Scan next display grid
		token = strtok(NULL, " ");
	}

	printf("frame sent\n");

	// Memory clean-up
	free(input);
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
