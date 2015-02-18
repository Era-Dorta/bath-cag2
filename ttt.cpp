#include <algorithm>
#include <string>
#include <iostream>
#include "tree.hh"
#include "Node.h"

using namespace std;

typedef tree<Node>::iterator TreeIt;
typedef tree<Node>::sibling_iterator SiblingIt;

std::ostream & operator<<(std::ostream & os, const Node & nd) {
	os << "r: " << nd.getR() << " v: " << nd.getV() << std::endl;
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			os << nd.getBoard(i, j);
		}
		os << std::endl;
	}
	return os;
}

std::ostream & operator<<(std::ostream & os, const Board & board) {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			os << board.getBoard(i, j);
		}
		os << std::endl;
	}
	return os;
}

char switchTurn(char turn) {
	if (turn == 'x') {
		return 'o';
	}
	return 'x';
}

void buildNode(tree<Node>& tr, TreeIt nodeIt, char turn, unsigned int nextI,
		unsigned int nextJ) {
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

void buildTree(tree<Node>& tr, char turn) {
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

SiblingIt getNextOptimalNode(const SiblingIt& startNode,
		const SiblingIt& endSib) {

	SiblingIt nextSib = startNode, nextNode = startNode;

	while (++nextSib != endSib) {
		if (nextSib->getV() > nextNode->getV()) {
			nextNode = nextSib;
		}
	}

	return nextNode;
}

int main(int, char **) {
	tree<Node> tr;
	//const TreeIt root = tr.begin();
	//tree<Node>::sibling_iterator nextSib;
	//TreeIt nextNd;

	tree_node_<Node> * currentNode;
	SiblingIt nextNode;

	buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;

	char turn = 'x';
	unsigned int maxGames = 10;
	const SiblingIt firstNode = tr.begin().node;
	Board board;
	for (unsigned int i = 1; i <= maxGames; i++) {
		currentNode = firstNode.node;

		// Get next move with max V
		nextNode = getNextOptimalNode(tr.begin().node, tr.end().node);

		// Make the play
		board.setBoard(nextNode->getA(), turn);

		cout << board << endl;

		turn = switchTurn(turn);
		currentNode = nextNode.node;

		while (!board.isFinalState()) {

			// Get next move with max V
			nextNode = getNextOptimalNode(currentNode->first_child,
					currentNode->last_child);

			// Make the play
			board.setBoard(nextNode->getA(), turn);

			cout << board << endl;

			turn = switchTurn(turn);
			currentNode = nextNode.node;
		}

		return 0;
	}

}
