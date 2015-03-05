/*
 * TreeBuilder.h
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#ifndef TREEHANDLER_H_
#define TREEHANDLER_H_

#include <stack>

#include "State.h"
#include "tree.hh"

typedef tree<State>::iterator NodeIt;
typedef tree<State>::sibling_iterator SiblingIt;

class TreeHandler {
public:
	TreeHandler();
	virtual ~TreeHandler();

	void buildTree(tree<State>& tr, char turn);

	SiblingIt getNextMove(char turn, float epsilon, const SiblingIt& startNode);

	void updateQ(float alpha, float gamma);

private:
	void updateV(float alpha, float gamma);

	void buildNode(tree<State>& tr, NodeIt nodeIt, char turn,
			unsigned int nextI, unsigned int nextJ);

	SiblingIt getNextOptimalNode(const SiblingIt& startNode);

	SiblingIt getNextExploreNode(const SiblingIt& startNode);

	float randf();

private:
	char cTurn;
	std::stack<NodeIt> optimalMoveStack;
	std::stack<NodeIt> optimalMoveParentStack;
};

#endif /* TREEHANDLER_H_ */
