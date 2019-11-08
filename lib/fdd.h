#ifndef __FDD_H__
#define __FDD_H__

#include <stdio.h>
#include <stdlib.h>

#include "spi/spi.h"
#include "img/display.h"
#include "img/read_png.h"
#include "text/read_txt.h"

#include "tb.h"

#define FRAME_PERIOD 67000
#define FRAME_FILENAME "./tmp/frame%d.png"

void ConfigureFDD(char* filename);
void DisplayFrames(char* filename, DisplayGrid* grid);
void DisplayText(char* filename, DisplayGrid* grid);
void VertMirrorDisplay(char* pixels);
void HorizMirrorDisplay(char* pixels);
void RotateClockwise(char* pixels);

#endif // __FDD_H__
