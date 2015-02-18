/*
 * TreeBuilder.h
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#ifndef TREEHANDLER_H_
#define TREEHANDLER_H_

#include "tree.hh"
#include "Node.h"

typedef tree<Node>::iterator TreeIt;
typedef tree<Node>::sibling_iterator SiblingIt;

class TreeHandler {
public:
	TreeHandler();
	virtual ~TreeHandler();

	void buildTree(tree<Node>& tr, char turn);

	SiblingIt getNextMove(float epsilon, const SiblingIt& startNode,
			const SiblingIt& endNode);

	void updateV(float alpha, tree_node_<Node> * currentNode);

private:
	void buildNode(tree<Node>& tr, TreeIt nodeIt, char turn, unsigned int nextI,
			unsigned int nextJ);

	SiblingIt getNextOptimalNode(const SiblingIt& startNode,
			const SiblingIt& endNode);

	SiblingIt getNextExploreNode(const SiblingIt& startNode,
			const SiblingIt& endNode);

	float randf();
};

#endif /* TREEHANDLER_H_ */
