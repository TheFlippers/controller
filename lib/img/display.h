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
	int len;
} DisplayList;

typedef struct {
	int width;
	int height;
	uint8_t** dispIds;
} DisplayGrid;

Display* CreateDisplay(Neighbors* data);
DisplayList* CreateList();
int InsertDisplay(DisplayList* list, Display* disp);
void FreeList(DisplayList* list);
Display* FindScreenSize(DisplayList* list, int* width, int* height);
DisplayGrid* CreateDisplayGrid(DisplayList* list);
void FreeDisplayGrid(DisplayGrid* grid);
int SaveDisplayGrid(char* filename, int width, int height);


#endif // __DISPLAY_H__
