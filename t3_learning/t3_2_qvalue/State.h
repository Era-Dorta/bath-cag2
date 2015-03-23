/*
 * Node.h
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#ifndef STATE_H_
#define STATE_H_

#include "Board.h"

class State {
public:
	State();
	State(float r, float v, const Board& board);
	virtual ~State();

	State(const State &otherNode);
	void operator=(const State &right);

	void computeFinalState(char turn);

	bool isFinalState();

	char getBoard(unsigned int i, unsigned int j) const;
	void setBoard(unsigned int i, unsigned int j, char val);
	void setBoard(unsigned int a, char val);

	float getR() const;
	void setR(float r);
	float getV() const;
	void setV(float v);
	unsigned int getA() const;
	void setA(unsigned int a);
	void setA(unsigned int i, unsigned int j);
private:
	float r;
	float v;

	unsigned int a;
	Board board;
};

#endif /* STATE_H_ */
