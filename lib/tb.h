#ifndef __TB_H__
#define __TB_H__

#include <stdio.h>

#include "spi/spi.h"
#include "img/display.h"

void PrintNeighbors(Neighbors* data);
void PrintDisplay(Display* data);

void TestCreateDisplay(void);

#endif // __TB_H__
