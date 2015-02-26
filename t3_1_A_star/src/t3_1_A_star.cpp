////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "AStarSearch.h"
#include "MapSearchNode.h"
#include "Map.h"

using namespace std;

// Global data

char toChar(int x) {
	switch (x) {
	case 1:
		return '.';
	case 9:
		return '#';
	case 10:
		return 'S';
	case 11:
		return '*';
	case 12:
		return 'E';
	default:
		return 'R';
	}
}

// Main

int main(int argc, char *argv[]) {
	// Our sample problem defines the world as a 2d array representing a terrain
	// Each element contains an integer from 0 to 5 which indicates the cost
	// of travel across the terrain. Zero means the least possible difficulty
	// in travelling (think ice rink if you can skate) whilst 5 represents the
	// most difficult. 9 indicates that we cannot pass.

	// Create an instance of the search class...
	Map map;

	AStarSearch astarsearch;

	unsigned int SearchCount = 0;

	const unsigned int NumSearches = 1;

	while (SearchCount < NumSearches) {

		// Create a start state
		MapSearchNode nodeStart;
		nodeStart.x = rand() % MAP_WIDTH;
		nodeStart.y = rand() % MAP_HEIGHT;

		// Define the goal state
		MapSearchNode nodeEnd;
		nodeEnd.x = rand() % MAP_WIDTH;
		nodeEnd.y = rand() % MAP_HEIGHT;

		// Set Start and goal states

		astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

		unsigned int SearchState;
		unsigned int SearchSteps = 0;

		do {
			SearchState = astarsearch.SearchStep();

			SearchSteps++;

		} while (SearchState == AStarSearch::SEARCH_STATE_SEARCHING);

		if (SearchState == AStarSearch::SEARCH_STATE_SUCCEEDED) {
			cout << "Search found goal state\n";

			MapSearchNode *node = astarsearch.GetSolutionStart();

			int steps = 0;

			std::vector<int> solution_map(map.getWorldMap());

			solution_map[node->x + node->y * MAP_WIDTH] = 10;
			//node->PrintNodeInfo();
			for (;;) {
				node = astarsearch.GetSolutionNext();

				if (!node) {
					break;
				}
				solution_map[node->x + node->y * MAP_WIDTH] = 11;
				steps++;

			};

			solution_map[nodeEnd.x + nodeEnd.y * MAP_WIDTH] = 12;

			for (int i = 0; i < MAP_WIDTH; i++) {
				for (int j = 0; j < MAP_HEIGHT; j++) {
					cout << toChar(solution_map[i + j * MAP_WIDTH]);
				}
				cout << endl;
			}

			// Once you're done with the solution you can free the nodes up
			astarsearch.FreeSolutionNodes();

		} else if (SearchState == AStarSearch::SEARCH_STATE_FAILED) {
			cout << "Search terminated. Did not find goal state\n";

		}

		SearchCount++;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
