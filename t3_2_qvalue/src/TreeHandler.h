/*
 * TreeBuilder.h
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#ifndef TREEHANDLER_H_
#define TREEHANDLER_H_

#include <stack>

#include "tree.hh"
#include "Node.h"

typedef tree<Node>::iterator NodeIt;
typedef tree<Node>::sibling_iterator SiblingIt;

class TreeHandler {
public:
	TreeHandler();
	virtual ~TreeHandler();

	void buildTree(tree<Node>& tr, char turn);

	SiblingIt getNextMove(char turn, float epsilon, const SiblingIt& startNode);

	void updateV(float alpha);

private:
	void buildNode(tree<Node>& tr, NodeIt nodeIt, char turn, unsigned int nextI,
			unsigned int nextJ);

	SiblingIt getNextOptimalNode(const SiblingIt& startNode);

	SiblingIt getNextExploreNode(const SiblingIt& startNode);

	float randf();

private:
	char cTurn;
	std::stack<NodeIt> optimalMoveStack;
	std::stack<NodeIt> optimalMoveParentStack;
};

#endif /* TREEHANDLER_H_ */
