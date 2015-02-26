/*
 * AStarSearch.h
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#ifndef ASTARSEARCH_H_
#define ASTARSEARCH_H_

// used for text debugging
#include <iostream>
#include <stdio.h>
#include <assert.h>

// stl includes
#include <algorithm>
#include <set>
#include <vector>
#include <cfloat>

using namespace std;

#include "MapSearchNode.h"

// The AStar search class. MapSearchNode is the users state space type
class AStarSearch {

public:
	// data

	enum {
		SEARCH_STATE_NOT_INITIALISED,
		SEARCH_STATE_SEARCHING,
		SEARCH_STATE_SUCCEEDED,
		SEARCH_STATE_FAILED,
		SEARCH_STATE_OUT_OF_MEMORY,
		SEARCH_STATE_INVALID
	};

	// A node represents a possible state in the search
	// The user provided state type is included inside this type

public:
	class Node {
	public:

		Node *parent; // used during the search to record the parent of successor nodes
		Node *child; // used after the search for the application to view the search in reverse

		float g; // cost of this node + it's predecessors
		float h; // heuristic estimate of distance to goal
		float f; // sum of cumulative cost of predecessors and self and heuristic

		Node();

		MapSearchNode m_UserState;
	};

	typedef vector<Node *>::iterator NodeIt;

	// For sorting the heap the STL needs compare function that lets us compare
	// the f value of two nodes

	class HeapCompare_f {
	public:

		bool operator()(const Node *x, const Node *y) const;
	};

public:
	// methods

	// constructor just initialises private data
	AStarSearch();

	AStarSearch(int MaxNodes);

	// call at any time to cancel the search and free up all the memory
	void CancelSearch();

	// Set Start and goal states
	void SetStartAndGoalStates(MapSearchNode &Start, MapSearchNode &Goal);

	// Advances search one step
	unsigned int SearchStep();

	// User calls this to add a successor to a list of successors
	// when expanding the search frontier
	bool AddSuccessor(MapSearchNode &State);

	// Free the solution nodes
	// This is done to clean up all used Node memory when you are done with the
	// search
	void FreeSolutionNodes();

	// Functions for traversing the solution

	// Get start node
	MapSearchNode *GetSolutionStart();

	// Get next node
	MapSearchNode *GetSolutionNext();

	// Get end node
	MapSearchNode *GetSolutionEnd();

	// Step solution iterator backwards
	MapSearchNode *GetSolutionPrev();

	// Get final cost of solution
	// Returns FLT_MAX if goal is not defined or there is no solution
	float GetSolutionCost();

	// Get the number of steps
	int GetStepCount();

private:
	// methods

	// This is called when a search fails or is cancelled to free all used
	// memory
	void FreeAllNodes();

	// This call is made by the search class when the search ends. A lot of nodes may be
	// created that are still present when the search ends. They will be deleted by this
	// routine once the search ends
	void FreeUnusedNodes();

	// Node memory management
	Node *AllocateNode();

	void FreeNode(Node *node);

private:
	// data

	// Heap (simple vector but used as a heap, cf. Steve Rabin's game gems article)
	vector<Node *> m_OpenList;

	// Closed list is a vector.
	vector<Node *> m_ClosedList;

	// Successors is a vector filled out by the user each type successors to a node
	// are generated
	vector<Node *> m_Successors;

	// State
	unsigned int m_State;

	// Counts steps
	int m_Steps;

	// Start and goal state pointers
	Node *m_Start;
	Node *m_Goal;

	Node *m_CurrentSolutionNode;

	// debugging : count memory allocation and free's
	int m_AllocateNodeCount;

	bool m_CancelRequest;
};

#endif /* ASTARSEARCH_H_ */
