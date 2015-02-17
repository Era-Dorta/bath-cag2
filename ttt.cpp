#include <algorithm>
#include <string>
#include <iostream>
#include "tree.hh"
#include "Node.h"

using namespace std;

typedef tree<Node>::iterator TreeIt;

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

	if (nodeIt->isFinalState(turn)) {
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

int main(int, char **) {
	tree<Node> tr;
	//const TreeIt root = tr.begin();
	//tree<Node>::sibling_iterator nextSib;
	//TreeIt nextNd;

	tree_node_<Node> * currentNode;
	tree<Node>::sibling_iterator nextSib, endSib, nextNode;

	buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;

	char turn = 'x';
	unsigned int maxGames = 10;
	float maxV = -100;
	const tree<Node>::sibling_iterator firstNode = tr.begin().node;
	Board board;
	for (unsigned int i = 1; i <= maxGames; i++) {
		currentNode = firstNode.node;

		maxV = -100;

		// Get next move with max V
		nextSib = tr.begin().node;
		endSib = tr.end().node;
		nextNode = nextSib;
		maxV = nextNode->getV();
		while (++nextSib != endSib) {
			if (nextSib->getV() > maxV) {
				maxV = nextSib->getV();
				nextNode = nextSib;
			}
		}

		// Make the play
		board.setBoard(nextNode->getA(), turn);

		cout << board << endl;

		turn = switchTurn(turn);
		currentNode = nextNode.node;

		while (!board.isFinalState()) {
			maxV = -100;

			// Get next move with max V
			nextSib = currentNode->first_child;
			endSib = currentNode->last_child;
			nextNode = nextSib;
			maxV = nextNode->getV();
			while (++nextSib != endSib) {
				if (nextSib->getV() > maxV) {
					maxV = nextSib->getV();
					nextNode = nextSib;
				}
			}

			// Make the play
			board.setBoard(nextNode->getA(), turn);

			cout << board << endl;

			turn = switchTurn(turn);
			currentNode = nextNode.node;
		}

		return 0;
	}

}
