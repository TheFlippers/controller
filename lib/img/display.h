#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdio.h>
#include <stdlib.h>

#include "spi/spi.h"

#define NUM_NEIGHBORS 4

typedef struct _Display {
	uint8_t id;
	struct _Display* neighborDisps[NUM_NEIGHBORS];
	uint8_t neighborIds[NUM_NEIGHBORS];
} Display;

typedef struct _DisplayNode {
	Display* disp;
	struct _DisplayNode* next;
} DisplayNode;

typedef struct {
	DisplayNode* root;
} DisplayList;

Display* CreateDisplay(Neighbors* data);
DisplayList* CreateList();
void InsertDisplay(DisplayList* list, Display* disp);
void FreeList(DisplayList* list);

#endif // __DISPLAY_H__
