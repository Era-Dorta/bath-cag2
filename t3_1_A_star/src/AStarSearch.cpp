/*
 * AStarSearch.cpp
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#include "AStarSearch.h"

AStarSearch::Node::Node() :
		parent(0), child(0), g(0.0f), h(0.0f), f(0.0f) {
}

bool AStarSearch::HeapCompare_f::operator ()(const Node* x,
		const Node* y) const {
	return x->f > y->f;
}

AStarSearch::AStarSearch() :
		m_State(SEARCH_STATE_NOT_INITIALISED), m_Steps(0), m_Start(0), m_Goal(
		NULL), m_CurrentSolutionNode( NULL) {
}

void AStarSearch::SetStartAndGoalStates(MapSearchNode &Start,
		MapSearchNode &Goal) {

	m_Start = AllocateNode();
	m_Goal = AllocateNode();

	assert((m_Start != NULL && m_Goal != NULL));

	m_Start->m_StateNode = Start;
	m_Goal->m_StateNode = Goal;

	m_State = SEARCH_STATE_SEARCHING;

	// Initialise the AStar specific parts of the Start Node
	// The user only needs fill out the state information

	m_Start->g = 0;
	m_Start->h = m_Start->m_StateNode.GoalDistanceEstimate(m_Goal->m_StateNode);
	m_Start->f = m_Start->g + m_Start->h;
	m_Start->parent = 0;

	// Push the start node on the Open list

	m_OpenList.push_back(m_Start); // heap now unsorted

	// Sort back element into heap
	push_heap(m_OpenList.begin(), m_OpenList.end(), HeapCompare_f());

	// Initialise counter for search steps
	m_Steps = 0;
}

unsigned int AStarSearch::SearchStep() {
	// Firstly break if the user has not initialised the search
	assert(
			(m_State > SEARCH_STATE_NOT_INITIALISED)
					&& (m_State < SEARCH_STATE_INVALID));

	// Next I want it to be safe to do a searchstep once the search has succeeded...
	if ((m_State == SEARCH_STATE_SUCCEEDED)
			|| (m_State == SEARCH_STATE_FAILED)) {
		return m_State;
	}

	// Failure is defined as emptying the open list as there is nothing left to
	// search...
	if (m_OpenList.empty()) {
		FreeAllNodes();
		m_State = SEARCH_STATE_FAILED;
		return m_State;
	}

	// Incremement step count
	m_Steps++;

	// Pop the best node (the one with the lowest f)
	Node *n = m_OpenList.front(); // get pointer to the node
	pop_heap(m_OpenList.begin(), m_OpenList.end(), HeapCompare_f());
	m_OpenList.pop_back();

	// Check for the goal, once we pop that we're done
	if (n->m_StateNode.IsGoal(m_Goal->m_StateNode)) {
		// The user is going to use the Goal Node he passed in
		// so copy the parent pointer of n
		m_Goal->parent = n->parent;
		m_Goal->g = n->g;

		// A special case is that the goal was passed in as the start state
		// so handle that here
		if (false == n->m_StateNode.IsSameState(m_Start->m_StateNode)) {
			FreeNode(n);

			// set the child pointers in each node (except Goal which has no child)
			Node *nodeChild = m_Goal;
			Node *nodeParent = m_Goal->parent;

			do {
				nodeParent->child = nodeChild;

				nodeChild = nodeParent;
				nodeParent = nodeParent->parent;

			} while (nodeChild != m_Start); // Start is always the first node by definition

		}

		// delete nodes that aren't needed for the solution
		FreeUnusedNodes();

		m_State = SEARCH_STATE_SUCCEEDED;

		return m_State;
	} else // not goal
	{

		// We now need to generate the successors of this node
		// The user helps us to do this, and we keep the new nodes in
		// m_Successors ...

		m_Successors.clear(); // empty vector of successor nodes to n

		// User provides this functions and uses AddSuccessor to add each successor of
		// node 'n' to m_Successors
		std::vector<int> x, y;
		bool ret = n->m_StateNode.GetSuccessors(
				n->parent ? &n->parent->m_StateNode : NULL, x, y);

		for (unsigned int i = 0; i < x.size(); i++) {
			MapSearchNode newNode(x[i], y[i]);
			AddSuccessor(newNode);
		}

		if (!ret) {

			NodeIt successor;

			// free the nodes that may previously have been added
			for (successor = m_Successors.begin();
					successor != m_Successors.end(); successor++) {
				FreeNode((*successor));
			}

			m_Successors.clear(); // empty vector of successor nodes to n

			// free up everything else we allocated
			FreeAllNodes();

			m_State = SEARCH_STATE_OUT_OF_MEMORY;
			return m_State;
		}

		// Now handle each successor to the current node ...
		for (NodeIt successor = m_Successors.begin();
				successor != m_Successors.end(); successor++) {

			// 	The g value for this successor ...
			float newg = n->g
					+ n->m_StateNode.GetCost((*successor)->m_StateNode);

			// Now we need to find whether the node is on the open or closed lists
			// If it is but the node that is already on them is better (lower g)
			// then we can forget about this successor

			// First linear search of open list to find node

			NodeIt openlist_result;

			for (openlist_result = m_OpenList.begin();
					openlist_result != m_OpenList.end(); openlist_result++) {
				if ((*openlist_result)->m_StateNode.IsSameState(
						(*successor)->m_StateNode)) {
					break;
				}
			}

			if (openlist_result != m_OpenList.end()) {

				// we found this state on open

				if ((*openlist_result)->g <= newg) {
					FreeNode((*successor));

					// the one on Open is cheaper than this one
					continue;
				}
			}

			NodeIt closedlist_result;

			for (closedlist_result = m_ClosedList.begin();
					closedlist_result != m_ClosedList.end();
					closedlist_result++) {
				if ((*closedlist_result)->m_StateNode.IsSameState(
						(*successor)->m_StateNode)) {
					break;
				}
			}

			if (closedlist_result != m_ClosedList.end()) {

				// we found this state on closed

				if ((*closedlist_result)->g <= newg) {
					// the one on Closed is cheaper than this one
					FreeNode((*successor));

					continue;
				}
			}

			// This node is the best node so far with this particular state
			// so lets keep it and set up its AStar specific data ...

			(*successor)->parent = n;
			(*successor)->g = newg;
			(*successor)->h = (*successor)->m_StateNode.GoalDistanceEstimate(
					m_Goal->m_StateNode);
			(*successor)->f = (*successor)->g + (*successor)->h;

			// Remove successor from closed if it was on it

			if (closedlist_result != m_ClosedList.end()) {
				// remove it from Closed
				FreeNode((*closedlist_result));
				m_ClosedList.erase(closedlist_result);

				// Fix thanks to ...
				// Greg Douglas <gregdouglasmail@gmail.com>
				// who noticed that this code path was incorrect
				// Here we have found a new state which is already CLOSED
				// anus

			}

			// Update old version of this node
			if (openlist_result != m_OpenList.end()) {

				FreeNode((*openlist_result));
				m_OpenList.erase(openlist_result);

				// re-make the heap
				// make_heap rather than sort_heap is an essential bug fix
				// thanks to Mike Ryynanen for pointing this out and then explaining
				// it in detail. sort_heap called on an invalid heap does not work
				make_heap(m_OpenList.begin(), m_OpenList.end(),
						HeapCompare_f());

			}

			// heap now unsorted
			m_OpenList.push_back((*successor));

			// sort back element into heap
			push_heap(m_OpenList.begin(), m_OpenList.end(), HeapCompare_f());

		}

		// push n onto Closed, as we have expanded it now

		m_ClosedList.push_back(n);

	} // end else (not goal so expand)

	return m_State; // Succeeded bool is false at this point.

}

bool AStarSearch::AddSuccessor(MapSearchNode &State) {
	Node *node = AllocateNode();

	if (node) {
		node->m_StateNode = State;

		m_Successors.push_back(node);

		return true;
	}

	return false;
}

void AStarSearch::FreeSolutionNodes() {
	Node *n = m_Start;

	if (m_Start->child) {
		do {
			Node *del = n;
			n = n->child;
			FreeNode(del);

			del = NULL;

		} while (n != m_Goal);

		FreeNode(n); // Delete the goal

	} else {
		// if the start node is the solution we need to just delete the start and goal
		// nodes
		FreeNode(m_Start);
		FreeNode(m_Goal);
	}

}

MapSearchNode *AStarSearch::GetSolutionStart() {
	m_CurrentSolutionNode = m_Start;
	if (m_Start) {
		return &m_Start->m_StateNode;
	} else {
		return NULL;
	}
}

MapSearchNode *AStarSearch::GetSolutionNext() {
	if (m_CurrentSolutionNode) {
		if (m_CurrentSolutionNode->child) {

			Node *child = m_CurrentSolutionNode->child;

			m_CurrentSolutionNode = m_CurrentSolutionNode->child;

			return &child->m_StateNode;
		}
	}

	return NULL;
}

MapSearchNode *AStarSearch::GetSolutionEnd() {
	m_CurrentSolutionNode = m_Goal;
	if (m_Goal) {
		return &m_Goal->m_StateNode;
	} else {
		return NULL;
	}
}

MapSearchNode *AStarSearch::GetSolutionPrev() {
	if (m_CurrentSolutionNode) {
		if (m_CurrentSolutionNode->parent) {

			Node *parent = m_CurrentSolutionNode->parent;

			m_CurrentSolutionNode = m_CurrentSolutionNode->parent;

			return &parent->m_StateNode;
		}
	}

	return NULL;
}

float AStarSearch::GetSolutionCost() {
	if (m_Goal && m_State == SEARCH_STATE_SUCCEEDED) {
		return m_Goal->g;
	} else {
		return FLT_MAX;
	}
}

int AStarSearch::GetStepCount() {
	return m_Steps;
}

void AStarSearch::FreeAllNodes() {
	// iterate open list and delete all nodes
	NodeIt iterOpen = m_OpenList.begin();

	while (iterOpen != m_OpenList.end()) {
		Node *n = (*iterOpen);
		FreeNode(n);

		iterOpen++;
	}

	m_OpenList.clear();

	// iterate closed list and delete unused nodes
	NodeIt iterClosed;

	for (iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end();
			iterClosed++) {
		Node *n = (*iterClosed);
		FreeNode(n);
	}

	m_ClosedList.clear();

	// delete the goal

	FreeNode(m_Goal);
}

void AStarSearch::FreeUnusedNodes() {
	// iterate open list and delete unused nodes
	NodeIt iterOpen = m_OpenList.begin();

	while (iterOpen != m_OpenList.end()) {
		Node *n = (*iterOpen);

		if (!n->child) {
			FreeNode(n);

			n = NULL;
		}

		iterOpen++;
	}

	m_OpenList.clear();

	// iterate closed list and delete unused nodes
	NodeIt iterClosed;

	for (iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end();
			iterClosed++) {
		Node *n = (*iterClosed);

		if (!n->child) {
			FreeNode(n);
			n = NULL;

		}
	}

	m_ClosedList.clear();
}

AStarSearch::Node *AStarSearch::AllocateNode() {
	Node *p = new Node;
	return p;
}

void AStarSearch::FreeNode(Node *node) {
	delete node;
}
