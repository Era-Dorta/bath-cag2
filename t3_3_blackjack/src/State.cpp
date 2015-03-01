/*
 * Node.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#include <cmath>
#include <iostream>

#include "State.h"

const unsigned int State::maxTake = 21;

State::State() {
	r = 0;
	v = 0;
	a = TAKE;
	pCardSum = 0;
	bCardSum = 0;
	turn = 'p';
	settleState = BOTH_TAKING;
}

State::State(float r, float v, Action a, unsigned int pCardSum,
		unsigned int bCardSum, char turn, SettleState settleState) {
	this->r = r;
	this->v = v;
	this->a = a;
	this->pCardSum = pCardSum;
	this->bCardSum = bCardSum;
	this->turn = turn;
	this->settleState = settleState;

	if (a == SETTLE) {
		switch (settleState) {
		case BOTH_TAKING: {
			this->settleState = ONE_SETTLE;
			break;
		}
		case ONE_SETTLE: {
			this->settleState = BOTH_SETTLE;
			break;
		}
		case BOTH_SETTLE: {
			std::cerr << "Settle action when both players already settled"
					<< std::endl;
			break;
		}
		}
	}
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

bool State::isFinalState() {
	return r != 0;
}

void State::computeFinalState(char turn) {
	if (pCardSum > 21) {
		r = 10;
		v = r;
		return;
	}
	if (bCardSum > 21) {
		r = -10;
		v = r;
		return;
	}

	if (settleState == BOTH_SETTLE) {
		if (pCardSum >= bCardSum) {
			r = 10;
			v = r;
			return;
		} else {
			r = -10;
			v = r;
			return;
		}
	}
}

void State::setV(float v) {
	this->v = v;
}

Action State::getA() const {
	return a;
}

void State::setA(Action a) {
	this->a = a;
}

void State::addCard(unsigned int cardVal) {
	if (turn == 'p') {
		pCardSum += cardVal;
	} else {
		bCardSum += cardVal;
	}
}

unsigned int State::getCardSum() const {
	if (turn == 'p') {
		return pCardSum;
	} else {
		return bCardSum;
	}
}

void State::setCardSum(unsigned int cardSum) {
	if (turn == 'p') {
		pCardSum = cardSum;
	} else {
		bCardSum = cardSum;
	}
}

char State::getTurn() const {
	return turn;
}

void State::setTurn(char turn) {
	this->turn = turn;
}

SettleState State::getSettleState() const {
	return settleState;
}

void State::setSettleState(SettleState settleState) {
	this->settleState = settleState;
}
