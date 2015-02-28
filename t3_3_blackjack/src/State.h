/*
 * Node.h
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#ifndef STATE_H_
#define STATE_H_

enum Action {TAKE, SETTLE};

class State {
public:
	State();
	State(float r, float v, Action a, unsigned int cardSum, char turn,
			bool pSettled, bool bSettled);
	virtual ~State();

	void computeFinalState(char turn);

	bool isFinalState();

	float getR() const;
	void setR(float r);
	float getV() const;
	void setV(float v);
	Action getA() const;
	void setA(Action a);
	unsigned int getCardSum() const;
	void setCardSum(unsigned int cardSum);
	void addCard(unsigned int cardVal);
	bool isSettled() const;
	void setSettled(bool settled);
	bool isSettled() const;
	void setSettled(bool settled);
	char getTurn() const;
	void setTurn(char turn);

private:
	float r;
	float v;

	Action a;
	unsigned int cardSum;
	char turn;
	bool pSettled;
	bool bSettled;

	const static unsigned int maxTake;
};

#endif /* STATE_H_ */
