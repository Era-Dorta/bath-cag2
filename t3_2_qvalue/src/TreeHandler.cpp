/*
 * TreeBuilder.cpp
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#include "TreeHandler.h"

#include "ExtraFun.h"

TreeHandler::TreeHandler() {
	cTurn = 'x';
}

TreeHandler::~TreeHandler() {
}

void TreeHandler::buildTree(tree<State>& tr, char turn) {
	State emptyNode;
	NodeIt root, nodeIt, nodeIt1;
	char nexTurn = switchTurn(turn);

	// Insert a root node with an empty board for convenience
	root = tr.insert(tr.begin(), emptyNode);

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			nodeIt = tr.append_child(root, emptyNode);
			nodeIt->setBoard(i, j, turn);
			nodeIt->setA(i, j);
			nodeIt->setV(0.5);
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

SiblingIt TreeHandler::getNextMove(char turn, float epsilon,
		const SiblingIt& startNode) {

	cTurn = turn;

	if (cTurn == 'o' || randf() > epsilon) {
		// Get next move with max V
		return getNextOptimalNode(startNode);
	} else {
		return getNextExploreNode(startNode);
	}
}

void TreeHandler::updateV(double alpha) {
	NodeIt parentNode, currentNode;

	while (!optimalMoveStack.empty()) {
		currentNode = optimalMoveStack.top();
		parentNode = optimalMoveParentStack.top();
		parentNode->setV(
				parentNode->getV()
						+ (float) alpha
								* (currentNode->getV() - parentNode->getV()));
		optimalMoveStack.pop();
		optimalMoveParentStack.pop();
	}
}

void TreeHandler::buildNode(tree<State>& tr, NodeIt nodeIt, char turn,
		unsigned int nextI, unsigned int nextJ) {
	nodeIt->setBoard(nextI, nextJ, turn);
	nodeIt->setA(nextI, nextJ);

	if (turn == 'x') {
		nodeIt->setV(0.5);
	} else {
		nodeIt->setV(float(rand() % 9));
	}

	nodeIt->computeFinalState(turn);

	if (nodeIt->isFinalState()) {
		return;
	}

	turn = switchTurn(turn);

	State newNode(*nodeIt);

	NodeIt node1;
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			if (nodeIt->getBoard(i, j) == 'e') {
				node1 = tr.append_child(nodeIt, newNode);
				buildNode(tr, node1, turn, i, j);
			}
		}
	}
}

SiblingIt TreeHandler::getNextOptimalNode(const SiblingIt& startNode) {

	SiblingIt nextSib = startNode, nextNode = startNode;

	// Advance to compare with the next sibling
	nextSib++;

	// Find the sibling with the bigger V
	for (; nextSib != startNode.end(); ++nextSib) {
		if (nextSib->getV() > nextNode->getV()) {
			nextNode = nextSib;
		}
	}

	if (cTurn == 'x' && nextNode.node->parent
			&& nextNode.node->parent->parent) {
		optimalMoveStack.push(nextNode);
		optimalMoveParentStack.push(nextNode.node->parent->parent);
	} else if (cTurn == 'o' && nextNode.number_of_children() == 0) {
		optimalMoveStack.push(nextNode);
		optimalMoveParentStack.push(nextNode.node->parent);
	}
	return nextNode;
}

SiblingIt TreeHandler::getNextExploreNode(const SiblingIt& startNode) {

	SiblingIt nextNode = startNode;

	unsigned int numSibling = NodeIt(startNode.parent_).number_of_children();

	// Pick a sibling at random
	numSibling = rand() % numSibling;

	nextNode += numSibling;

	return nextNode;
}

float TreeHandler::randf() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
