/*
 * MapSearchNode.h
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#ifndef MAPSEARCHNODE_H_
#define MAPSEARCHNODE_H_

#include <vector>

class MapSearchNode {
public:
	int x;	 // the (x,y) positions of the node
	int y;

	MapSearchNode();
	MapSearchNode(int px, int py);

	float GoalDistanceEstimate(MapSearchNode &nodeGoal);
	bool IsGoal(MapSearchNode &nodeGoal);
	bool GetSuccessors(MapSearchNode *parent_node, std::vector<int>& newX,
			std::vector<int>& newY);
	float GetCost(MapSearchNode &successor);
	bool IsSameState(MapSearchNode &rhs);

	void PrintNodeInfo();

};

#endif /* MAPSEARCHNODE_H_ */
