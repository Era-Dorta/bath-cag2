/*
 * Node.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#include <cmath>

#include "State.h"

State::State() {
	r = 0;
	v = 0;
	a = 0;
}

State::State(float r, float v, const Board& board) {
	this->r = r;
	this->v = v;
	this->board = board;
	this->a = 0;
}

State::State(const State& otherNode) {
	r = otherNode.r;
	v = otherNode.v;
	a = otherNode.a;
	board = otherNode.board;
}

State::~State() {
}

float State::getR() const {
	return r;
}

void State::setR(float r) {
	this->r = r;
}

float State::getV() const {
	return v;
}

char State::getBoard(unsigned int i, unsigned int j) const {
	return board.getBoard(i, j);
}

void State::setBoard(unsigned int i, unsigned int j, char val) {
	board.setBoard(i, j, val);
}

void State::setBoard(unsigned int a, char val) {
	board.setBoard(a, val);
}

bool State::isFinalState() {
	return r != 0;
}

void State::computeFinalState(char turn) {
	r = board.checkFinalState(turn);

	if (r != 0) {
		v = r;
	}
}

void State::setV(float v) {
	this->v = v;
}

void State::operator =(const State& right) {
	r = right.r;
	v = right.v;
	a = right.a;

	board = right.board;
}

unsigned int State::getA() const {
	return a;
}

void State::setA(unsigned int a) {
	this->a = a;
}

void State::setA(unsigned int i, unsigned int j) {
	a = i * 3 + j;
}
