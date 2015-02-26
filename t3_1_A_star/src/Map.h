/*
 * Map.h
 *
 *  Created on: 26 Feb 2015
 *      Author: gdp24
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>

// The world map, map data in Map.cpp

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;

class Map {
public:
	Map();
	virtual ~Map();
	static int GetMap(int x, int y);
	std::vector<int> getWorldMap();
};

#endif /* MAP_H_ */
