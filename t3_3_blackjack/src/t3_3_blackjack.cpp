#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "TreeHandler.h"
#include "ExtraFun.h"
#include "State.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const tree_node_<State> *nd) {
	os << "r: " << nd->data.getR() << " v: " << nd->data.getV() << std::endl;
	return os;
}

std::ostream & operator<<(std::ostream & os, const State & nd) {
	os << "r: " << nd.getR() << " v: " << nd.getV() << " sum: "
			<< nd.getCardSum() << std::endl;
	return os;
}

int main(int, char **) {
	/* initialize random seed: */
	srand(0);

	// Set precision for easy formatting
	std::cout.precision(2);
	std::cout << std::fixed;

	TreeHandler treeHandler;
	tree<State> tr;

	treeHandler.buildTree(tr, 'p');
	cout << "done building size " << tr.size() << endl;

	char turn = 'p', other = 'b';
	unsigned int maxGames = 1000000;
	float epsilon = (float) 0.2;
	float alpha = (float) 0.1;

	tree_node_<State> * currentNode;
	const SiblingIt firstNode = tr.begin().node;
	SiblingIt nextNode;

	unsigned int numGames;
	for (numGames = 1; numGames <= maxGames; numGames++) {
		currentNode = firstNode.node;
		//cout << "next turn " << turn << endl;
		//cout << currentNode << endl;

		while (currentNode != NULL) {

			// Get next move with max V
			nextNode = treeHandler.getNextMove(turn, epsilon,
					currentNode->first_child);

			//	cout << "next turn " << other << endl;
			//	cout << nextNode.node << endl;

			other = turn;
			turn = switchTurn(turn);
			currentNode = nextNode.node;
		}

		treeHandler.updateV(alpha);

		turn = 'x';
		other = 'o';

		//cout << "---------- Game end ----------------" << endl;
	}
	numGames--;

	cout << "Played " << numGames << " games" << endl;

}
