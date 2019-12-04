#ifndef __FDD_H__
#define __FDD_H__

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "spi/spi.h"
#include "img/display.h"
#include "img/read_png.h"
#include "text/read_txt.h"

#include "tb.h"

#define CONFIG_DELAY 10
#define FRAME_PERIOD 67000
#define FIFO_PATH "../script/php/fdd_fifo"
#define MAX_READ 200
#define CONFIG_FILENAME "./fdd_conf.txt"
#define FRAME_FILENAME "./output/frame%d.png"
#define COUNT_FILENAME "./output/num_frames.txt"
#define TEXT_FILENAME "./text/message.txt"

typedef struct {
	DisplayGrid* grid;
	char* data;
} Arguments;

void RunFDD(void);
void* ReadFromFIFO(void* args);
DisplayGrid* ConfigureFDD(void);
void* DisplayImages(void* args);
void* DisplayText(void* args);
void* DisplayFrame(void* args);
void VertMirrorDisplay(char* pixels);
void HorizMirrorDisplay(char* pixels);

#endif // __FDD_H__
