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

typedef struct {
	int modulesWidth;
	int modulesHeight;
	char ***modulePixels;	
} DisplayPixels;

Image* ReadPNGFile(char* filename); 
DisplayPixels* ImageToDisplayPixels(Image* image);
void FreeImage(Image* image);

#endif // __READ_PNG_H__
