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

	void updateV(float alpha);

private:
	void buildNode(tree<State>& tr, NodeIt nodeIt, char turn, Action a,
			unsigned int cardNum);

	SiblingIt getNextOptimalNode(const SiblingIt& startNode);

	SiblingIt getNextExploreNode(const SiblingIt& startNode);

	float randf();

	void resetCardDeck();

private:
	char cTurn;
	std::vector<unsigned int> cards;
	std::stack<NodeIt> optimalMoveStack;
	std::stack<NodeIt> optimalMoveParentStack;
};

#endif /* TREEHANDLER_H_ */
