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
	a = 0;
}

Node::Node(float r, float v, const Board& board) {
	this->r = r;
	this->v = v;
	this->board = board;
	this->a = 0;
}

Node::Node(const Node& otherNode) {
	r = otherNode.r;
	v = otherNode.v;
	a = otherNode.a;
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
	return board.getBoard(i, j);
}

void Node::setBoard(unsigned int i, unsigned int j, char val) {
	board.setBoard(i, j, val);
}

void Node::setBoard(unsigned int a, char val) {
	board.setBoard(a, val);
}

bool Node::isFinalState() {
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
	a = right.a;

	board = right.board;
}

unsigned int Node::getA() const {
	return a;
}

void Node::setA(unsigned int a) {
	this->a = a;
}

void Node::setA(unsigned int i, unsigned int j) {
	a = i * 3 + j;
}
