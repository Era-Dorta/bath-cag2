/*
 * AStartState.h
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#ifndef ASTARTSTATE_H_
#define ASTARTSTATE_H_

#include "stlastar.h"

template<class T> class AStarState {
public:
	virtual ~AStarState() {
	}
	virtual float GoalDistanceEstimate(T &nodeGoal) = 0; // Heuristic function which computes the estimated cost to the goal node
	virtual bool IsGoal(T &nodeGoal) = 0; // Returns true if this node is the goal node
	virtual bool GetSuccessors(AStarSearch<T> *astarsearch, T *parent_node) = 0; // Retrieves all successors to this node and adds them via astarsearch.addSuccessor()
	virtual float GetCost(T &successor) = 0; // Computes the cost of travelling from this node to the successor node
	virtual bool IsSameState(T &rhs) = 0; // Returns true if this node is the same as the rhs node
};

#endif /* ASTARTSTATE_H_ */
