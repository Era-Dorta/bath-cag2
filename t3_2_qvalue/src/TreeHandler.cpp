/*
 * TreeBuilder.cpp
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#include "TreeHandler.h"

#include "ExtraFun.h"

TreeHandler::TreeHandler() {
}

TreeHandler::~TreeHandler() {
}

void TreeHandler::buildTree(tree<Node>& tr, char turn) {
	Node emptyNode;
	TreeIt root, nodeIt, nodeIt1;
	char nexTurn = switchTurn(turn);

	// Insert a root node with an empty board for convenience
	root = tr.insert(tr.begin(), emptyNode);

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			nodeIt = tr.append_child(root, emptyNode);
			nodeIt->setBoard(i, j, turn);
			nodeIt->setA(i, j);
			for (unsigned int k = 0; k < 3; k++) {
				for (unsigned int l = 0; l < 3; l++) {
					if (nodeIt->getBoard(k, l) == 'e') {
						nodeIt1 = tr.append_child(nodeIt, *nodeIt);
						buildNode(tr, nodeIt1, nexTurn, k, l);
					}
				}
			}
		}
	}
}

SiblingIt TreeHandler::getNextMove(float epsilon, const SiblingIt& startNode,
		const SiblingIt& endNode) {

	if (randf() > epsilon) {
		// Get next move with max V
		return getNextOptimalNode(startNode, endNode);
	} else {
		return getNextExploreNode(startNode, endNode);
	}
}

void TreeHandler::updateV(float alpha, tree_node_<Node> * currentNode) {
	tree_node_<Node> *updateNode = currentNode, *parentNode;

	// o won, update the other side of the tree
	if (currentNode->data.getR() < 0 && updateNode->parent != 0) {
		updateNode = updateNode->parent;
	}

	// Arrived at a new state, update parent if current state value is
	// bigger that parent value
	while (updateNode->parent != 0 && updateNode->parent->parent != 0) {
		parentNode = updateNode->parent->parent;
		if (parentNode->data.getV() < updateNode->data.getV()) {
			parentNode->data.setV(
					parentNode->data.getV() + alpha * updateNode->data.getV()
							- parentNode->data.getV());
		} else {
			return;
		}
		updateNode = parentNode;
	}

	/* Update code with sibling value check
	 SiblingIt firstSib, lastSib;
	 while(updateNode->parent != 0 && updateNode->parent->parent != 0) {
	 parentNode = updateNode->parent->parent;

	 firstSib = updateNode->parent->first_child;
	 lastSib = updateNode->parent->last_child;

	 while (firstSib != updateNode) {
	 if (firstSib->getV() > updateNode->data.getV()) {
	 return;
	 }
	 firstSib++;
	 }

	 while (lastSib != updateNode) {
	 if (lastSib->getV() > updateNode->data.getV()) {
	 return;
	 }
	 lastSib--;
	 }

	 if (parentNode->data.getV() < updateNode->data.getV()) {
	 parentNode->data.setV(
	 parentNode->data.getV() + alpha * updateNode->data.getV()
	 - parentNode->data.getV());
	 } else {
	 return;
	 }
	 updateNode = parentNode;
	 }*/
}

void TreeHandler::buildNode(tree<Node>& tr, TreeIt nodeIt, char turn,
		unsigned int nextI, unsigned int nextJ) {
	nodeIt->setBoard(nextI, nextJ, turn);
	nodeIt->setA(nextI, nextJ);

	nodeIt->computeFinalState(turn);

	if (nodeIt->isFinalState()) {
		return;
	}

	turn = switchTurn(turn);

	Node newNode(*nodeIt);

	TreeIt node1;
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			if (nodeIt->getBoard(i, j) == 'e') {
				node1 = tr.append_child(nodeIt, newNode);
				buildNode(tr, node1, turn, i, j);
			}
		}
	}
}

SiblingIt TreeHandler::getNextOptimalNode(const SiblingIt& startNode,
		const SiblingIt& endNode) {

	SiblingIt nextSib = startNode, nextNode = startNode;

	while (++nextSib != endNode) {
		if (nextSib->getV() > nextNode->getV()) {
			nextNode = nextSib;
		}
	}

	return nextNode;
}

SiblingIt TreeHandler::getNextExploreNode(const SiblingIt& startNode,
		const SiblingIt& endNode) {

	SiblingIt nextSib = startNode, nextNode = startNode;
	unsigned int numSibling = 0;
	while (++nextSib != endNode) {
		numSibling++;
	}

	nextSib = startNode;
	nextNode = startNode;

	// Pick a sibling at random
	numSibling = rand() % numSibling;
	unsigned int i = 0;

	while (i < numSibling) {
		nextNode++;
	}

	return nextNode;
}

float TreeHandler::randf() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
