#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include "TreeHandler.h"
#include "Board.h"
#include "ExtraFun.h"
#include "State.h"

using namespace std;

int main(int, char **) {

	unsigned int nTrainGames = 1000000;

	// Exploration rate, 1 for exploring, 0 for greedy policy
	double epsilon;

	// Learning rate, 1 will update the states a lot, 0 will not update,
	// decrease on each step for convergence
	float alpha = (float) 1;

	// Gamma is the discount factor, 0 for immediate rewards, 1 for future
	// rewards
	float gamma = (float) 1;

	// ------------------------------------
	// ------------- TRAINING -------------
	// ------------------------------------

	// Set precision for easy formatting
	std::cout.precision(2);
	std::cout << std::fixed;

	Board board;
	TreeHandler treeHandler;
	tree<State> tr;

	float expectedGain;
	unsigned int nWin, nLoose, nDraw;

	for (epsilon = 0.0; epsilon <= 1.0; epsilon += 0.1) {

		// Rebuild the tree state space
		srand(12);
		char turn = 'x';
		tr.clear();
		treeHandler.buildTree(tr, turn);
		const SiblingIt firstNode = tr.begin().node;
		tree_node_<State> * currentNode;
		SiblingIt nextNode;

		nWin = 0;
		nLoose = 0;
		nDraw = 0;
		expectedGain = 0.0;

		for (unsigned int i = 1; i <= nTrainGames; i++) {
			currentNode = firstNode.node;
			//cout << "next turn " << turn << endl;
			//cout << currentNode << endl;
			turn = 'x';

			board.reset();

			while (!board.isFinalState()) {

				// Get next move, either maxV or explore move
				nextNode = treeHandler.getNextMove(turn, (float) epsilon,
						currentNode->first_child);

				// Make the play
				board.setBoard(nextNode->getA(), turn);

				//cout << "next turn " << switchTurn(turn) << endl;
				//cout << nextNode.node << endl;

				treeHandler.updateQ(alpha / (float) i, gamma);

				turn = switchTurn(turn);
				currentNode = nextNode.node;
			}

			//cout << currentNode << endl;

			treeHandler.updateQ(alpha / (float) i, gamma);

			expectedGain += currentNode->data.getR();

			if (currentNode->data.getR() == 10) {
				nWin++;
			} else if (currentNode->data.getR() == -10) {
				nLoose++;
			} else {
				nDraw++;
			}

			//cout << "---------- Game end ----------------" << endl;
		}

		expectedGain = expectedGain / (float) nTrainGames;

		cout << "Qlearning with " << nTrainGames << " games, explore rate: "
				<< epsilon * 100 << "%" << endl;
		cout << "Mean gain: " << expectedGain << " win: " << nWin << ", loose: "
				<< nLoose << ", draw: " << nDraw << endl << endl;
	}
}
