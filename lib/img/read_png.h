#ifndef __READ_PNG_H__
#define __READ_PNG_H__

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#define MODULE_SIZE 7

typedef struct {
	int width;
	int height;
	png_bytep *bytes;
} Image;

Image* ReadPNGFile(char* filename); 
char* ImageToDisplayPixels(Image* image, int w, int h, int x, int y);
void FreeImage(Image* image);

#endif // __READ_PNG_H__
