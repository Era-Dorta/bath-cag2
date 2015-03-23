/*
 * ExtraFun.h
 *
 *  Created on: 18 Feb 2015
 *      Author: gdp24
 */

#ifndef EXTRAFUN_H_
#define EXTRAFUN_H_

#include <iostream>

inline char switchTurn(char turn) {
	if (turn == 'x') {
		return 'o';
	}
	return 'x';
}

#endif /* EXTRAFUN_H_ */
