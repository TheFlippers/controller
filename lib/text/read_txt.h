#ifndef __READ_TXT_H__
#define __READ_TXT_H__

#include <stdio.h>
#include <stdlib.h>

#include "img/display.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 7
#define FONT_OFFSET 32

typedef struct {
	char character;
	char pixels[7];
} Character;

typedef struct {
	int len;
	Character* letters;
} Message;

char* ReadTXTFile(char* filename);
int TextToPixels(char* text, Message* msg);
void FreeMessage(Message* msg);

#endif // __READ__TXT_H__
