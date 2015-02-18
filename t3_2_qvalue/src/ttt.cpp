#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "Node.h"
#include "TreeHandler.h"
#include "Board.h"
#include "ExtraFun.h"

using namespace std;

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

	Board board;
	TreeHandler treeHandler;
	tree<Node> tr;
	//const TreeIt root = tr.begin();
	//tree<Node>::sibling_iterator nextSib;
	//TreeIt nextNd;

	tree_node_<Node> * currentNode;
	SiblingIt nextNode;

	treeHandler.buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;

	char turn = 'x';
	unsigned int maxGames = 10;
	float epsilon = (float)0.2;
	float alpha = (float)0.1;

	const SiblingIt firstNode = tr.begin().node;

	for (unsigned int i = 1; i <= maxGames; i++) {
		currentNode = firstNode.node;

		nextNode = treeHandler.getNextMove(epsilon, tr.begin().node, tr.end().node);

		// Make the play
		board.setBoard(nextNode->getA(), turn);

		cout << board << endl;

		turn = switchTurn(turn);
		currentNode = nextNode.node;
		//updateV(currentNode);

		while (!board.isFinalState()) {

			// Get next move with max V
			nextNode = treeHandler.getNextMove(epsilon, currentNode->first_child,
					currentNode->last_child);

			// Make the play
			board.setBoard(nextNode->getA(), turn);

			cout << board << endl;

			turn = switchTurn(turn);
			currentNode = nextNode.node;

			treeHandler.updateV(alpha, currentNode);
		}

		return 0;
	}

}
