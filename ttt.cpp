#include <algorithm>
#include <string>
#include <iostream>
#include "tree.hh"
#include "Node.h"

using namespace std;

char board[3][3];

typedef tree<Node>::iterator TreeIt;

char switchTurn(char turn) {
	if (turn == 'x') {
		return 'o';
	}
	return 'x';
}

void buildNode(tree<Node>& tr, TreeIt nodeIt, char turn,
		unsigned int nextI, unsigned int nextJ) {
	nodeIt->setBoard(nextI, nextJ, turn);

	if (nodeIt->checkFinalState(turn) != 0) {
		return;
	}

	turn = switchTurn(turn);

	Node newNode(*nodeIt);

	TreeIt node1;
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			if (nodeIt->getBoard(i, j) == 'e') {
				node1 = tr.insert(nodeIt, newNode);
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
			emptyNode.setBoard(i, j, turn);
			nodeIt = tr.insert(root, emptyNode);
			for (unsigned int k = 0; k < 3; k++) {
				for (unsigned int l = 0; l < 3; l++) {
					if (emptyNode.getBoard(k, l) == 'e') {
						nodeIt1 = tr.insert(nodeIt, emptyNode);
						buildNode(tr, nodeIt1, nexTurn, k, l);
					}
				}
			}
			emptyNode.setBoard(i, j, 'e');
		}
	}
}

int main(int, char **) {
	tree<Node> tr;
	buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;
}
