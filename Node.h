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
	float getR() const;
	void setR(float r);
	float getV() const;
	void setV(float v);

private:
	float r;
	float v;

	Board board;
};

#endif /* NODE_H_ */
