/*
 * Node.h
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	Node();
	Node(float r, float v, const char board[3][3]);
	virtual ~Node();

	Node(const Node &otherNode);
	void operator=(const Node &right );

	float checkFinalState(char turn);

	char getBoard(unsigned int i, unsigned int j) const;
	void setBoard(unsigned int i, unsigned int j, char val);
	float getR() const;
	void setR(float r);
	float getV() const;
	void setV(float v);

private:
	bool checkHori(char turn, unsigned int i) const;
	bool checkVert(char turn, unsigned int i) const;
	bool checkDiag(char turn) const;
	bool isFull() const;

private:
		float r;
		float v;
		char board[3][3];
};

#endif /* NODE_H_ */
