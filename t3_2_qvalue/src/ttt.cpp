#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "Node.h"
#include "TreeHandler.h"
#include "Board.h"
#include "ExtraFun.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const tree_node_<Node> *nd) {
	float values[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

	SiblingIt nextSib, endSib;

	nextSib = nd->first_child;
	endSib = nextSib.end();

	for (; nextSib != endSib; ++nextSib) {
		unsigned int a = nextSib->getA();
		values[a / 3][a % 3] = nextSib->getV();
	}

	os << "r: " << nd->data.getR() << " v: " << nd->data.getV() << std::endl;
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			os << nd->data.getBoard(i, j);
		}
		os << "\t";
		for (unsigned int j = 0; j < 3; j++) {
			os << values[i][j] << " ";
		}
		os << std::endl;
	}
	return os;
}

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

int main(int, char **) {
	/* initialize random seed: */
	srand(0);

	// Set precision for easy formatting
	std::cout.precision(2);
	std::cout << std::fixed;

	Board board;
	TreeHandler treeHandler;
	tree<Node> tr;

	treeHandler.buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;

	char turn = 'x', other = 'o';
	unsigned int maxGames = 10;
	float epsilon = (float) 0.2;
	float alpha = (float) 0.1;

	tree_node_<Node> * currentNode;
	const SiblingIt firstNode = tr.begin().node;
	SiblingIt nextNode;

	for (unsigned int i = 1; i <= maxGames; i++) {
		currentNode = firstNode.node;
		cout << "next turn " << turn << endl;
		cout << currentNode << endl;

		while (!board.isFinalState()) {

			// Get next move with max V
			nextNode = treeHandler.getNextMove(turn, epsilon,
					currentNode->first_child);

			// Make the play
			board.setBoard(nextNode->getA(), turn);

			cout << "next turn " << other << endl;
			cout << nextNode.node << endl;

			other = turn;
			turn = switchTurn(turn);
			currentNode = nextNode.node;
		}

		treeHandler.updateV(alpha);

		board.reset();

		turn = 'x';
		other = 'o';

		cout << "---------- Game end ----------------" << endl;
	}

}
