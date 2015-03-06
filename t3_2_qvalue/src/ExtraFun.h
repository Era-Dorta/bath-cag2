/*
 * ExtraFun.h
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#ifndef EXTRAFUN_H_
#define EXTRAFUN_H_

#include <iostream>

inline char switchTurn(char turn) {
	if (turn == 'x') {
		return 'o';
	}
	return 'x';
}

/*inline std::ostream & operator<<(std::ostream & os, const tree_node_<State> *nd) {
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

inline std::ostream & operator<<(std::ostream & os, const State & nd) {
	os << "r: " << nd.getR() << " v: " << nd.getV() << std::endl;
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			os << nd.getBoard(i, j);
		}
		os << std::endl;
	}
	return os;
}

inline std::ostream & operator<<(std::ostream & os, const Board & board) {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			os << board.getBoard(i, j);
		}
		os << std::endl;
	}
	return os;
}*/

#endif /* EXTRAFUN_H_ */
