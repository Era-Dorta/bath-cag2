// This code is based on the A* implementation in
// https://github.com/justinhj/astar-algorithm-cpp

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
		return 'G';
	default:
		return 'R';
	}
}

void printPath(Map& map, MapSearchNode& nodeEnd, AStarSearch& astarsearch) {
	MapSearchNode* node = astarsearch.GetSolutionStart();
	std::vector<int> solution_map(map.getWorldMap());
	solution_map[node->y * MAP_HEIGHT + node->x] = 10;
	for (;;) {
		node = astarsearch.GetSolutionNext();
		if (!node) {
			break;
		}
		solution_map[node->y * MAP_HEIGHT + node->x] = 11;
	};
	solution_map[nodeEnd.y * MAP_WIDTH + nodeEnd.x] = 12;
	cout << " X 00000000001111111111" << endl;
	cout << "   ";
	for (int i = 0; i < MAP_WIDTH; i++) {
		cout << i % 10;
	}
	cout << endl << "Y                     " << endl;
	unsigned int k = 0;
	for (int i = 0; i < MAP_WIDTH; i++) {
		cout << k << i % 10 << " ";
		for (int j = 0; j < MAP_HEIGHT; j++) {
			cout << toChar(solution_map[i * MAP_HEIGHT + j]);
		}
		cout << endl;
		if (i == 9) {
			k = 1;
		}
	}
}

// Main

int main() {
	// Our sample problem defines the world as a 2d array representing a terrain
	// Each element contains an integer from 0 to 5 which indicates the cost
	// of travel across the terrain. Zero means the least possible difficulty
	// in travelling whilst 5 represents the most difficult. 9 indicates that
	// we cannot pass.
	// While printing . are 1, # are 9, S is the start position, * is the a
	// node in the path and G is the goal
	// Map coordinates are like image coordinates
	// 0 -- X -- Width->
	// |
	// Y
	// |
	// Height

	Map map;
	AStarSearch astarsearch;

	// Create a start state
	MapSearchNode nodeStart;
	nodeStart.x = 0;
	nodeStart.y = 0;

	// Define the goal state
	MapSearchNode nodeEnd;
	nodeEnd.x = 10;
	nodeEnd.y = 8;

	// Set Start and goal states

	astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

	unsigned int SearchState;
	unsigned int SearchSteps = 0;

	do {
		SearchState = astarsearch.SearchStep();

		SearchSteps++;

	} while (SearchState == AStarSearch::SEARCH_STATE_SEARCHING);

	if (SearchState != AStarSearch::SEARCH_STATE_SUCCEEDED) {
		cout << "Search terminated. Did not find goal state\n";
		return 0;
	}

	cout << "Search found goal state\n";

	printPath(map, nodeEnd, astarsearch);

	astarsearch.FreeSolutionNodes();

	return 0;
}
