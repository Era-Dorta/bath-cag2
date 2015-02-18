/*
 * TreeBuilder.cpp
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#include "TreeHandler.h"

#include "ExtraFun.h"

TreeHandler::TreeHandler() {
	// TODO Auto-generated constructor stub

}

TreeHandler::~TreeHandler() {
	// TODO Auto-generated destructor stub
}

void TreeHandler::buildTree(tree<Node>& tr, char turn) {
	Node emptyNode;
	TreeIt root, nodeIt, nodeIt1;
	root = tr.begin();
	char nexTurn = switchTurn(turn);

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			nodeIt = tr.insert(root, emptyNode);
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

void TreeHandler::updateV(float alpha, tree_node_<Node> * currentNode){
	tree_node_<Node> * updateNode = currentNode, siblingNode;

	if(updateNode->parent != 0 && updateNode->parent->parent != 0){
		updateNode = updateNode->parent->parent;
		if(updateNode->data.getV() < currentNode->data.getV()){
			updateNode->data.setV(updateNode->data.getV() + alpha *
					currentNode->data.getV() - updateNode->data.getV());
		} else {
			return;
		}
	} else {
		return;
	}
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

float TreeHandler::randf(){
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
