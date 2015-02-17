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
}

Node::Node(float r, float v, const Board& board) {
	this->r = r;
	this->v = v;
	this->board = board;
}

Node::Node(const Node& otherNode) {
	r = otherNode.r;
	v = otherNode.v;
	board = otherNode.board;
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
	return board.getBoard(i,j);
}

void Node::setBoard(unsigned int i, unsigned int j, char val) {
	board.setBoard(i, j, val);
}

bool Node::isFinalState(char turn) {
	return r != 0;
}

void Node::computeFinalState(char turn) {
	r = board.checkFinalState(turn);
}

void Node::setV(float v) {
	this->v = v;
}

void Node::operator =(const Node& right) {
	r = right.r;
	v = right.v;

	board = right.board;
}
