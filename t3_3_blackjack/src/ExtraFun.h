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
	if (turn == 'p') {
		return 'b';
	}
	return 'p';
}

#endif /* EXTRAFUN_H_ */
