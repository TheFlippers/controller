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

	return list;
}

void InsertDisplay(DisplayList* list, Display* disp) {

	DisplayNode* new = NULL;
	DisplayNode* curr = NULL;
	
	// Create new node to insert into list
	new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "ERROR: Could not allocate memory!\n");
		return;
	}

	// Initialize list node
	new->disp = disp;

	// Add node to head of list
	new->next = list->root;
	list->root = new;

	// Update display neighbors for members in list
	curr = new->next;
	while(curr != NULL) {
		for (int i = 0; i < NUM_NEIGHBORS; i++) {
			if (disp->neighborIds[i] = curr->disp->id) {
				disp->neighborDisps[i] = curr->disp;
				// TODO: set neighbor's neighbor to new display
			}
		}
	}
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
