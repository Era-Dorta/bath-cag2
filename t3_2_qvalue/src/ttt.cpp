#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

#include <map>
#include <vector>
#include <cmath>
#include <cfloat>

#include "gnuplot-iostream.h"

#include "TreeHandler.h"
#include "Board.h"
#include "ExtraFun.h"
#include "State.h"

using namespace std;

int main(int, char **) {

	unsigned int nTrainGames = 100000;

	// Exploration rate, 1 for exploring, 0 for greedy policy
	double epsilon;

	// Learning rate, 1 will update the states a lot, 0 will not update,
	// decrease on each step for convergence
	double alpha = 1.0;

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

	// Plotting variables
	Gnuplot gp;
	std::vector<std::pair<double, double> > expectedGainV, firstPlayVal[9];

	for (epsilon = 0; epsilon <= 1.0; epsilon += 0.1) {

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

				turn = switchTurn(turn);
				currentNode = nextNode.node;
			}

			//cout << currentNode << endl;

			treeHandler.updateV(alpha / (double) i);

			expectedGain += currentNode->data.getR();

			if (currentNode->data.getR() == 10) {
				nWin++;
			} else if (currentNode->data.getR() == -10) {
				nLoose++;
			} else {
				nDraw++;
			}

			if (i % 100 == 0) {
				SiblingIt cNode = firstNode.node->first_child;

				for (unsigned int k = 0; k < 3; k++) {
					for (unsigned int l = 0; l < 3; l++) {
						//cout << cNode->getV() << endl;
						firstPlayVal[k * 3 + l].push_back(
								std::make_pair(i, cNode->getV()));
						cNode++;
					}
				}
			}
			//cout << endl;

			//cout << "---------- Game end ----------------" << endl;
		}

		expectedGain = expectedGain / (float) nTrainGames;

		expectedGainV.push_back(std::make_pair(epsilon, expectedGain));

		cout << "Qlearning with " << nTrainGames << " games, explore rate: "
				<< epsilon * 100 << "%" << endl;
		cout << "Mean gain: " << expectedGain << " win: " << nWin << ", loose: "
				<< nLoose << ", draw: " << nDraw << endl;

		SiblingIt cNode = tr.begin().node->first_child;
		unsigned int maxTile = 0;
		float maxV = cNode->getV();

		for (unsigned int k = 0; k < 9; k++) {
			if (cNode->getV() > maxV) {
				maxTile = k;
				maxV = cNode->getV();
			}
			cNode++;
		}

		cout << "Max tile " << maxTile << ", with V " << maxV << endl;
		cout << endl;

		if (epsilon == 0.2 || epsilon == 0.4 || epsilon == 0.6) {
			gp << "set xrange [0:" << nTrainGames << "]\nset yrange [0.4:10]"
					<< endl;
			gp << "set key off" << endl;
			gp
					<< "plot '-' with lines, '-' with lines, '-' with lines , \
'-' with lines , '-' with lines , '-' with lines, '-' with lines , \
'-' with lines , '-' with lines"
					<< endl;

			for (unsigned int i = 0; i < 9; i++) {
				gp.send1d(firstPlayVal[i]);
			}
		}

		for (unsigned int i = 0; i < 9; i++) {
			firstPlayVal[i].clear();
		}
	}

	gp << "plot '-' with lines title 'Expected Gain'\n";
	gp.send1d(expectedGainV);
}
