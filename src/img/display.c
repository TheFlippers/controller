#include "img/display.h"

Display* CreateDisplay(Neighbors* data) {

	Display* disp = NULL;

	// Allocate memory for display structure
	disp = malloc(sizeof(*disp));
	if (disp == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return disp;
	}

	// Initialize fields based on input data
	disp->id = data->id;
	for(int i = 0; i < NUM_NEIGHBORS; i++) {
		disp->neighborDisps[i] = NULL;
		disp->neighborIds[i] = data->neighbors[i];
	}

	return disp;
}

DisplayList* CreateList() {
	
	DisplayList* list = NULL;

	// Allocate memory for list
	list = malloc(sizeof(*list));
	if (list == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return list;
	}

	// Initialize empty list
	list->root = NULL;
	list->len = 0;

	return list;
}

int InsertDisplay(DisplayList* list, Display* disp) {

	DisplayNode* new = NULL;
	DisplayNode* curr = NULL;
	
	// Create new node to insert into list
	new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return -1;
	}

	// Initialize list node
	new->disp = disp;

	// Add node to head of list
	new->next = list->root;
	list->root = new;
	list->len += 1;

	// Update display neighbors for members in list
	for (int i = 0; i < NUM_NEIGHBORS; i++) {
		if (new->disp->neighborIds[i] != 0) {
			curr = new->next;
			while (curr != NULL) {
				if (new->disp->neighborIds[i] == curr->disp->id) {
					new->disp->neighborDisps[i] = curr->disp;
					curr->disp->neighborDisps[NUM_NEIGHBORS - i - 1] = new->disp;
				}
				curr = curr->next;
			}
		}
	}

	return 0;
}

void FreeList(DisplayList* list) {
	
	DisplayNode* curr = NULL;
	DisplayNode* next = NULL;

	// Iterate over list to free members
	curr = list->root;
	while(curr != NULL) {
		next = curr->next;
		free(curr->disp);
		free(curr);
		curr = next;
	}
	free(list);
}

Display* FindScreenSize(DisplayList* list, int* width, int* height) {

	Display* curr = NULL;
	Display* corner = NULL;
	int count = 0;

	// Check screen exists
	if (list->root == NULL) {
		fprintf(stderr, "ERROR: Screen contains no displays!\n");
		return NULL;
	}
	curr = list->root->disp;

	// Traverse grid starting at root to upper left corner
	while (curr->neighborDisps[NORTH] != NULL) {
		curr = curr->neighborDisps[NORTH];
	}
	while (curr->neighborDisps[WEST] != NULL) {
		curr = curr->neighborDisps[WEST];
	}
	corner = curr;

	// Move across display to measure width
	while (curr != NULL) {
		count++;
		curr = curr->neighborDisps[EAST];
	}
	*width = count;

	// Return to corner
	count = 0;
	curr = corner;

	// Move across display to measure height
	while (curr != NULL) {
		count++;
		curr = curr->neighborDisps[SOUTH];
	}
	*height = count;

	return corner;
}

DisplayGrid* CreateDisplayGrid(DisplayList* list) {

	DisplayGrid* screen = NULL;
	Display* curr = NULL;
	Display* corner = NULL;

	// Allocate memory for screen
	screen = malloc(sizeof(*screen));
	if (screen == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return screen;
	}

	// Calculate size of screen
	corner = FindScreenSize(list, &(screen->width), &(screen->height));

	// Allocate memory for display rows
	screen->dispIds = malloc(sizeof(*(screen->dispIds)) * screen->width);
	if (screen->dispIds == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		free(screen);
		return NULL;
	}

	// Allocate memory for display columns
	for (int x = 0; x < screen->width; x++) {
		screen->dispIds[x] = malloc(sizeof(*(screen->dispIds[x])) * screen->height);
		if (screen->dispIds[x] == NULL) {
			fprintf(stderr, "ERROR: Could not allocate memory!\n");
			for (int i = 0; i < x; i++) {
				free(screen->dispIds[i]);
			}
			free(screen->dispIds);
			free(screen);
			return NULL;
		}
	}

	// Traverse displays to find ID values
	for (int x = 0; x < screen->width; x++) {
		curr = corner;
		for (int y = 0; y < screen->height; y++) {
			screen->dispIds[x][y] = curr->id; 
			curr = curr->neighborDisps[SOUTH];
		}
		corner = corner->neighborDisps[EAST];
	}

	return screen;
}

void FreeDisplayGrid(DisplayGrid* grid) {

	// Free columns
	for (int x = 0; x < grid->width; x++) {
		free(grid->dispIds[x]);
	}

	// Free remaining memory
	free(grid->dispIds);
	free(grid);
}
