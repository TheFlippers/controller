#ifndef __TB_H__
#define __TB_H__

#include <stdio.h>

#include "spi/spi.h"
#include "img/display.h"

void PrintNeighbors(Neighbors* data);
void PrintDisplay(Display* data);
void PrintDisplayList(DisplayList* list);
void PrintArray(char* data, int len);

void TestSPITransfer(void);
void TestSPISend(void);
void TestSPIRead(void);
void TestCreateDisplay(void);
void TestCreateDisplayList(void);

#endif // __TB_H__
