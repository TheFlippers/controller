#include "img/read_png.h"

Image* ReadPNGFile(char* filename) {

	FILE *fp = NULL;
	png_structp png;
	png_infop info;
	int width, height;
	png_byte color_type, bit_depth;
	Image* image = NULL;
	png_bytep *row_pointers = NULL;

	// Read file
	fp = fopen(filename, "rb");

	// Read png data
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		fprintf(stderr, "ERROR: Culd not read png data!\n");
		fclose(fp);
		return NULL;
	}

	// Read png info
	info = png_create_info_struct(png);
	if (!info) {
		fprintf(stderr, "ERROR: Could not read png info!\n");
		fclose(fp);
		png_destroy_read_struct(&png, NULL, NULL);
		return NULL;
	}

	// Initialize png structures
	png_init_io(png, fp);
	png_read_info(png, info);

	// Get image parameters
	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	// Check image parameters
	if (width % MODULE_SIZE != 0 || height % MODULE_SIZE != 0) {
		fprintf(stderr, "ERROR: Incorrect png dimensions!\n");
		fclose(fp);
		png_destroy_read_struct(&png, &info, NULL);
		return NULL;
	}
	if (color_type != PNG_COLOR_TYPE_GRAY || bit_depth != 1) {
		fprintf(stderr, "ERROR: Incorrect png format!\n");
		fclose(fp);
		png_destroy_read_struct(&png, &info, NULL);
		return NULL;
	}

	// Convert png data to readable format
	png_set_expand_gray_1_2_4_to_8(png);

	// Allocate memory for image rows
	row_pointers = malloc(sizeof(*row_pointers) * height);
	if (row_pointers == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		fclose(fp);
		png_destroy_read_struct(&png, &info, NULL);
		return NULL;
	}

	// Allocate memory for each column
	for (int i = 0; i < height; i++) {
		row_pointers[i] = malloc(png_get_rowbytes(png, info));
		if (row_pointers[i] == NULL) {
			fprintf(stderr, "ERROR: Could not allocate memory!\n");
			fclose(fp);
			png_destroy_read_struct(&png, &info, NULL);
			for (int j = 0; j < i; j++) {
				free(row_pointers[j]);
			}
			free(row_pointers);
			return NULL;
		}
	}

	// Read image
	png_read_image(png, row_pointers);


	// Save image data
	image = malloc(sizeof(*image));
	if (image == NULL) {
			fprintf(stderr, "ERROR: Could not allocate memory!\n");
			fclose(fp);
			png_destroy_read_struct(&png, &info, NULL);
			for (int i = 0; i < height; i++) {
				free(row_pointers[i]);
			}
			free(row_pointers);
			return NULL;
	}
	image->width = width;
	image->height = height;
	image->bytes = row_pointers;
	
	// Cleanup
	fclose(fp);
	png_destroy_read_struct(&png, &info, NULL);

	return image;
}

char* ImageToDisplayPixels(Image* image, int w, int h, int x, int y) {
	/*
	DisplayPixels *disp = NULL;

	// Allocate memory for display
	disp = malloc(sizeof(*disp));
	if (disp == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return NULL;
	}

	// Calculate screen size in number of modules
	disp->modulesWidth = image->width / MODULE_SIZE;
	disp->modulesHeight = image->height / MODULE_SIZE;

	// Allocate memory for display modules
	disp->modulePixels = malloc(sizeof(*(disp->modulePixels)) * disp->modulesWidth);
	if (disp->modulePixels == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		free(disp);
		return NULL;
	}
	for (int x = 0; x < disp->modulesWidth; x++) {
		disp->modulePixels[x] = malloc(sizeof(*(disp->modulePixels[x])) * disp->modulesHeight);
		if (disp->modulePixels[x] == NULL) {
			fprintf(stderr, "ERROR: Could not allocate memory!\n");
			for (int i = 0; i < x; i++) {
				free(disp->modulePixels[i]);
			}
			free(disp->modulePixels);
			free(disp);
			return NULL;
		}
	}

	// Allocate pixel buffers
	for (int x = 0; x < disp->modulesWidth; x++) {
		for (int y = 0; y < disp->modulesHeight; y++) {
			disp->modulePixels[x][y] = malloc(sizeof(*(disp->modulePixels[x][y])) * MODULE_SIZE);
			if (disp->modulePixels[x][y] == NULL) {
				fprintf(stderr, "ERROR: Could not allocate memory!\n");
				for (int i = 0; i < x; i++) {
					for (int j = 0; j < y; j++) {
						free(disp->modulePixels[i][j]);
					}
				}
				for (int i = 0; i < disp->modulesWidth; i++) {
					free(disp->modulePixels[i]);
				}
				free(disp->modulePixels);
				free(disp);
				return NULL;
			}
		}
	}

	// Populate pixel data
	for (int x = 0; x < image->width; x++) {
		for (int y = 0; y < image->height; y++) {
			int moduleX = x / MODULE_SIZE;
			int moduleY = y / MODULE_SIZE;
			int moduleRow = y % MODULE_SIZE;
			int moduleCol = x % MODULE_SIZE;
			disp->modulePixels[moduleX][moduleY][moduleRow] |= (image->bytes[x][y] & 1) << (moduleCol);
		}
	}

	return disp;
	*/

	char *buf = NULL;
	int bitCnt = 0;
	int bit = 0;

	buf = malloc(sizeof(*buf) * MODULE_SIZE);
	if (buf == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return NULL;
	}

	for (int row = 0; row < MODULE_SIZE; row++) {
		bitCnt = 0;
		buf[row] = 0;
		for (int col = 0; col < MODULE_SIZE; col++) {
			bit = (image->bytes[row][col] == 0xFF) ? 1 : 0;
			buf[row] |= bit << col;
			bitCnt += bit;
		}
		if (bitCnt % 2 == 0) {
			buf[row] |= 1 << 7;
		}
	}

	return buf;
}

void FreeImage(Image* image) {

	// Free pixel data
	for (int i = 0; i < image->height; i++) {
		free(image->bytes[i]);
	}
	free(image->bytes);

	// Free iamge
	free(image);
}
