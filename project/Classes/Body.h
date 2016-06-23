#ifndef _BODY_H_
#define _BODY_H_

#include "cocos2d.h"
#include <vector>

typedef struct body {
public:
	std::vector<double> angle;
	std::vector<double> radius;
	double broundaryRadiu;
} Body;

const std::vector<double> angle = {
	-1 * M_PI / 6, 0, M_PI / 6,
	-1 * M_PI / 3, M_PI / 3,
	-1 * M_PI / 2, M_PI / 2,
	-1 * M_PI * 2 / 3, M_PI * 2 / 3,
	-1 * M_PI * 5 / 6, M_PI * 5 / 6, M_PI };
const std::vector<double> radius = { 50, 50, 50, 50, 50, 60, 50, 60, 50, 60, 50, 50 };

/*
 * We define an area of attacking in the shape of a sector.
 * And the angle is defined in a clockwise direction.
 * Maybe once we change the weapon of our player, we can use a new method of attacking.
 */

typedef struct attackArea {
	std::vector<cocos2d::Vec2> vec;
	std::vector<double> radius;
	std::vector<double> angle;
} AttackArea;

#endif