#ifndef __TB_H__
#define __TB_H__

#include <stdio.h>

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
void PrintDisplayPixels(DisplayPixels* disp);

void TestSPITransfer(void);
void TestSPISend(void);
void TestSPIRead(void);
void TestCreateDisplay(void);
void TestCreateDisplayList(void);
void TestCreateDisplayGrid(int w, int h);
void TestAutoConfigure();
void TestUpdatePixels();

#endif // __TB_H__
