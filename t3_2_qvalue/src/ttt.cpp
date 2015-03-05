#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "TreeHandler.h"
#include "Board.h"
#include "ExtraFun.h"
#include "State.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const tree_node_<State> *nd) {
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

std::ostream & operator<<(std::ostream & os, const State & nd) {
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
	tree<State> tr;

	treeHandler.buildTree(tr, 'x');
	cout << "done building size " << tr.size() << endl;

	char turn = 'x';
	unsigned int maxGames = 1000000;

	// Exploration rate, 1 for exploring, 0 for greedy policy
	float epsilon = (float) 0.2;

	// Learning rate, 1 will update the states a lot, 0 will not update,
	// decrease on each step for convergence
	float alpha = (float) 1;

	// Gamma is the discount factor, 0 for immediate rewards, 1 for future
	// rewards
	float gamma = (float) 1;

	tree_node_<State> * currentNode;
	const SiblingIt firstNode = tr.begin().node;
	SiblingIt nextNode;

	unsigned int numGames;
	for (numGames = 1; numGames <= maxGames; numGames++) {
		currentNode = firstNode.node;
		//cout << "next turn " << turn << endl;
		//cout << currentNode << endl;

		while (!board.isFinalState()) {

			// Get next move with max V
			nextNode = treeHandler.getNextMove(turn, epsilon,
					currentNode->first_child);

			// Make the play
			board.setBoard(nextNode->getA(), turn);

			//cout << "next turn " << switchTurn(turn) << endl;
			//cout << nextNode.node << endl;

			treeHandler.updateQ(alpha / (float) numGames, gamma);

			turn = switchTurn(turn);
			currentNode = nextNode.node;
		}

		treeHandler.updateQ(alpha / (float) numGames, gamma);

		board.reset();

		turn = 'x';

		//cout << "---------- Game end ----------------" << endl;
	}
	numGames--;

	cout << "Played " << numGames << " games" << endl;

}
