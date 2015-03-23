/*
 * Board.cpp
 *
 *  Created on: 17 Feb 2015
 *      Author: gdp24
 */

#include "Board.h"

Board::Board() {
	reset();
}

Board::~Board() {
}

Board& Board::operator =(const Board& right) {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			board[i][j] = right.board[i][j];
		}
	}
	return *this;
}

char Board::getBoard(unsigned int i, unsigned int j) const {
	return board[i][j];
}

void Board::setBoard(unsigned int i, unsigned int j, char val) {
	board[i][j] = val;
}

void Board::setBoard(unsigned int a, char val) {
	board[a / 3][a % 3] = val;
}

float Board::checkFinalState(char turn) {
	float rWin = 10, rLoose = -10, rDraw = 1;

	//Check wins
	for (unsigned int i = 0; i < 3; i++) {
		if (checkHori(turn, i) || checkVert(turn, i)) {
			if (turn == 'x') {
				return rWin;
			} else {
				return rLoose;
			}
		}
	}

	if (checkDiag(turn)) {
		if (turn == 'x') {
			return rWin;
		} else {
			return rLoose;
		}
	}

	//Draw
	if (isFull()) {
		return rDraw;
	}

	return 0;
}

bool Board::checkHori(char turn, unsigned int i) const {
	if (board[i][0] == turn && board[i][1] == turn && board[i][2] == turn) {
		return true;
	}
	return false;
}

bool Board::checkVert(char turn, unsigned int i) const {
	if (board[0][i] == turn && board[1][i] == turn && board[2][i] == turn) {
		return true;
	}
	return false;
}

bool Board::checkDiag(char turn) const {
	if (board[0][0] == turn && board[1][1] == turn && board[2][2] == turn) {
		return true;
	}
	if (board[0][2] == turn && board[1][1] == turn && board[2][0] == turn) {
		return true;
	}
	return false;
}

Board::Board(const Board& otherBoard) {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			board[i][j] = otherBoard.board[i][j];
		}
	}
}

void Board::reset() {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			board[i][j] = 'e';
		}
	}
}

bool Board::isFull() const {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			if (board[i][j] == 'e') {
				return false;
			}
		}
	}
	return true;
}

bool Board::isFinalState() {
	for (unsigned int i = 0; i < 3; i++) {
		if (checkHori('x', i) || checkVert('x', i) || checkHori('o', i)
				|| checkVert('o', i)) {
			return true;
		}
	}

	if (checkDiag('x') || checkDiag('o') || isFull()) {
		return true;
	}

	return false;
}
