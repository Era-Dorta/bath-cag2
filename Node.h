/*
 * Node.h
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#include "Board.h"

#ifndef NODE_H_
#define NODE_H_

#include <ostream>

class Node {
public:
	Node();
	Node(float r, float v, const Board& board);
	virtual ~Node();

	Node(const Node &otherNode);
	void operator=(const Node &right);

	void computeFinalState(char turn);

	bool isFinalState(char turn);

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

#endif /* NODE_H_ */
