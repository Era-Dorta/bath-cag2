/*
 * Node.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#include "Node.h"

Node::Node() {
	r = 0;
	v = 0;

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			this->board[i][j] = 'e';
		}
	}
}

Node::Node(float r, float v, const char board[3][3]) {
	this->r = r;
	this->v = v;

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			this->board[i][j] = board[i][j];
		}
	}
}

Node::Node(const Node& otherNode) {
	r = otherNode.r;
	v = otherNode.v;

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			board[i][j] = otherNode.board[i][j];
		}
	}
}

Node::~Node() {
}

float Node::getR() const {
	return r;
}

void Node::setR(float r) {
	this->r = r;
}

float Node::getV() const {
	return v;
}

char Node::getBoard(unsigned int i, unsigned int j) const {
	return board[i][j];
}

void Node::setBoard(unsigned int i, unsigned int j, char val) {
	board[i][j] = val;
}

void Node::setV(float v) {
	this->v = v;
}

void Node::operator =(const Node& right) {
	r = right.r;
	v = right.v;

	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			board[i][j] = right.board[i][j];
		}
	}
}

float Node::checkFinalState(char turn) {
	float rWin = 10, rLoose = -10, rDraw = 1;

	//Check wins
	for (unsigned int i = 0; i < 3; i++) {
		if (checkHori(turn, i) || checkVert(turn, i)) {
			if (turn == 'x') {
				r = rWin;
			} else {
				r = rLoose;
			}
			return r;
		}
	}

	if (checkDiag(turn)) {
		if (turn == 'x') {
			r = rWin;
		} else {
			r = rLoose;
		}
		return r;
	}

	//Draw
	if (isFull()) {
		r = rDraw;
	}

	return r;
}

bool Node::checkHori(char turn, unsigned int i) const {
	if (board[i][0] == turn && board[i][1] == turn && board[i][2] == turn) {
		return true;
	}
	return false;
}

bool Node::checkVert(char turn, unsigned int i) const {
	if (board[0][i] == turn && board[1][i] == turn && board[2][i] == turn) {
		return true;
	}
	return false;
}

bool Node::checkDiag(char turn) const {
	if (board[0][0] == turn && board[1][1] == turn && board[2][2] == turn) {
		return true;
	}
	if (board[0][2] == turn && board[1][1] == turn && board[2][0] == turn) {
		return true;
	}
	return false;
}

bool Node::isFull() const {
	for (unsigned int i = 0; i < 3; i++) {
		for (unsigned int j = 0; j < 3; j++) {
			if (board[i][j] == 'e') {
				return false;
			}
		}
	}
	return true;
}
