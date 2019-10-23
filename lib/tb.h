#ifndef __TB_H__
#define __TB_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "img/display.h"
#include "img/read_png.h"
#include "spi/spi.h"

void PrintNeighbors(Neighbors* data);
void PrintDisplay(Display* data);
void PrintDisplayList(DisplayList* list);
void PrintArray(char* data, int len);
void PrintDisplayGrid(DisplayGrid* grid);
void PrintPixels(char* pixels);
void PrintImage(Image* image);

void TestSPITransfer(void);
void TestSPISend(void);
void TestSPIRead(void);
void TestSendDisplayID(void);
void TestSendPixelData(void);
void TestCreateNeighbors(void);
void TestCreateDisplay(void);
void TestCreateDisplayList(void);
void TestCreateDisplayGrid(int w, int h, int printNeighbors);
void TestAutoConfigure();
void TestConvertImage(char *filename);
void TestFPS();
void TestUpdatePixels();

#endif // __TB_H__
