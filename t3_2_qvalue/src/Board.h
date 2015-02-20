/*
 * Board.h
 *
 *  Created on: 17 Feb 2015
 *      Author: gdp24
 */

#ifndef BOARD_H_
#define BOARD_H_

class Board {
public:
	Board();
	virtual ~Board();
	Board(const Board &otherBoard);

	Board& operator =(const Board& right);

	bool isFinalState();

	float checkFinalState(char turn);

	void reset();

	char getBoard(unsigned int i, unsigned int j) const;
	void setBoard(unsigned int i, unsigned int j, char val);
	void setBoard(unsigned int a, char val);

private:
	bool checkHori(char turn, unsigned int i) const;
	bool checkVert(char turn, unsigned int i) const;
	bool checkDiag(char turn) const;
	bool isFull() const;

private:
	char board[3][3];
};

#endif /* BOARD_H_ */
