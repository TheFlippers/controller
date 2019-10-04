#include "tb.h"

void PrintNeighbors(Neighbors* data) {
	printf("  %d  \n  |  \n%d-%d-%d\n  |  \n  %d  \n", data->neighbors[NORTH],
								   					   data->neighbors[WEST],
								   					   data->id,
								   					   data->neighbors[EAST],
													   data->neighbors[SOUTH]);
}

void PrintDisplay(Display* data) {
	printf("  %d  \n +-+ \n%d|%d|%d\n +-+ \n  %d  \n", data->neighborIds[NORTH],
								   					   data->neighborIds[WEST],
								   					   data->id,
								   					   data->neighborIds[EAST],
													   data->neighborIds[SOUTH]);
}

void TestCreateDisplay(void) {

	Neighbors input;
	Display* output;

	input.id = 1;
	input.neighbors[NORTH] = 0;
	input.neighbors[EAST] = 2;
	input.neighbors[WEST] = 0;
	input.neighbors[SOUTH] = 0;

	PrintNeighbors(&input);

	output = CreateDisplay(&input);

	PrintDisplay(output);

}
