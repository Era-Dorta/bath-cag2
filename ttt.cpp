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

char switchTurn(char turn) {
	if (turn == 'x') {
		return 'o';
	}
	return 'x';
}

void buildNode(tree<Node>& tr, TreeIt nodeIt, char turn, unsigned int nextI,
		unsigned int nextJ) {
	nodeIt->setBoard(nextI, nextJ, turn);

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
	const TreeIt root = tr.begin();
	buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;

	int maxGames = 10;
	Board board;
	for (unsigned int i = 1; i <= maxGames; i++) {
		//while (!board.isFinalState()) {

		//}
	}

}
