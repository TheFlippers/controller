#ifndef __TB_H__
#define __TB_H__

#include <stdio.h>

#include "spi/spi.h"
#include "img/display.h"

void PrintNeighbors(Neighbors* data);
void PrintDisplay(Display* data);
void PrintDisplayList(DisplayList* list);
void PrintArray(char* data, int len);
void PrintDisplayGrid(DisplayGrid* grid);

void TestSPITransfer(void);
void TestSPISend(void);
void TestSPIRead(void);
void TestCreateDisplay(void);
void TestCreateDisplayList(void);
void TestCreateDisplayGrid(int w, int h);

#endif // __TB_H__
