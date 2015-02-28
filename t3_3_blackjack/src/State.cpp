/*
 * Node.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#include <cmath>

#include "State.h"

unsigned int State::maxTake = 21;

State::State() {
	r = 0;
	v = 0;
	a = TAKE;
	cardSum = 0;
	turn = 'p';
	pSettled = false;
	bSettled = false;
}

State::State(float r, float v, Action a, unsigned int cardSum, char turn,
		bool pSettled, bool bSettled) {
	this->r = r;
	this->v = v;
	this->a = a;
	this->cardSum = cardSum;
	this->turn = turn;
	this->pSettled = pSettled;
	this->bSettled = bSettled;
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
	if(cardSum > 21){
		if(turn == 'p'){
			r = -10;
		} else {
			r = 10;
		}
		v = r;
		return;
	}

	if(a == SETTLE){

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

unsigned int State::getCardSum() const {
return cardSum;
}

void State::setCardSum(unsigned int cardSum) {
this->cardSum = cardSum;
}

void State::addCard(unsigned int cardVal) {
	cardSum += cardVal;
}

bool State::isSettled() const {
	return bSettled;
}

void State::setSettled(bool settled) {
	bSettled = settled;
}

bool State::isSettled() const {
	return pSettled;
}

void State::setSettled(bool settled) {
	pSettled = settled;
}

char State::getTurn() const {
	return turn;
}

void State::setTurn(char turn) {
	this->turn = turn;
}
