/*
 * Node.h
 *
 *  Created on: 16 Feb 2015
 *      Author: gdp24
 */

#ifndef STATE_H_
#define STATE_H_

enum Action {
	TAKE, SETTLE
};
enum SettleState {
	BOTH_TAKING, ONE_SETTLE, BOTH_SETTLE
};
class State {
public:
	State();
	State(float r, float v, Action a, unsigned int pCardSum,
			unsigned int bCardSum, char turn, SettleState settleState);
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
	char getTurn() const;
	void setTurn(char turn);
	SettleState getSettleState() const;
	void setSettleState(SettleState settleState);

private:
	float r;
	float v;

	Action a;
	unsigned int pCardSum;
	unsigned int bCardSum;
	char turn;
	SettleState settleState;

	static const unsigned int maxTake;
};

#endif /* STATE_H_ */
