/*
 * MapSearchNode.cpp
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#include "MapSearchNode.h"

#include "Map.h"

#include <cmath>
#include <iostream>
using namespace std;

MapSearchNode::MapSearchNode() {
	x = y = 0;
}

MapSearchNode::MapSearchNode(int px, int py) {
	x = px;
	y = py;
}

bool MapSearchNode::IsSameState(MapSearchNode &rhs) {

	// same state in a maze search is simply when (x,y) are the same
	if ((x == rhs.x) && (y == rhs.y)) {
		return true;
	} else {
		return false;
	}

}

void MapSearchNode::PrintNodeInfo() {
	cout << "Node position : (" << x << "," << y << ")" << endl;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal) {
	return fabsf(x - nodeGoal.x) + fabsf(y - nodeGoal.y);
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal) {

	if ((x == nodeGoal.x) && (y == nodeGoal.y)) {
		return true;
	}

	return false;
}

bool MapSearchNode::GetSuccessors(MapSearchNode *parent_node,
		std::vector<int>& newX, std::vector<int>& newY) {

	newX.clear();
	newY.clear();

	int parent_x = -1;
	int parent_y = -1;

	if (parent_node) {
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}

	// push each possible move except allowing the search to go backwards

	if ((Map::GetMap(x - 1, y) < 9)
			&& !((parent_x == x - 1) && (parent_y == y))) {
		newX.push_back(x - 1);
		newY.push_back(y);
	}

	if ((Map::GetMap(x, y - 1) < 9)
			&& !((parent_x == x) && (parent_y == y - 1))) {
		newX.push_back(x);
		newY.push_back(y - 1);
	}

	if ((Map::GetMap(x + 1, y) < 9)
			&& !((parent_x == x + 1) && (parent_y == y))) {
		newX.push_back(x + 1);
		newY.push_back(y);
	}

	if ((Map::GetMap(x, y + 1) < 9)
			&& !((parent_x == x) && (parent_y == y + 1))) {
		newX.push_back(x);
		newY.push_back(y + 1);
	}

	return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor) {
	return (float) Map::GetMap(x, y);

}

