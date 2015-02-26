/*
 * MapSearchNode.h
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#ifndef MAPSEARCHNODE_H_
#define MAPSEARCHNODE_H_

#include "stlastar.h"

class MapSearchNode {
public:
	int x;	 // the (x,y) positions of the node
	int y;

	MapSearchNode();
	MapSearchNode(int px, int py);

	float GoalDistanceEstimate(MapSearchNode &nodeGoal);
	bool IsGoal(MapSearchNode &nodeGoal);
	bool GetSuccessors(AStarSearch<MapSearchNode> *astarsearch,
			MapSearchNode *parent_node);
	float GetCost(MapSearchNode &successor);
	bool IsSameState(MapSearchNode &rhs);

	void PrintNodeInfo();

};

#endif /* MAPSEARCHNODE_H_ */
