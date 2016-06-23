/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM, amplified by nemoremold
 */

#include "EntityProperties.h"

EntityProperties::EntityProperties(void) {
	_valueMap[INITPOSITIONX] = 0;
	_valueMap[INITPOSITIONY] = 0;
	_valueMap[DEFAULTVELOCITY] = 0;
	_valueMap[VELOCITY] = 0;
	_valueMap[MAXHP] = 0;
	_valueMap[ACCELERATION] = 0;
	_valueMap[ANGULARVELOCITY] = 0;
	_valueMap[ORIENTATION] = 0;
	_valueMap[DEFENCE] = 0;
	_valueMap[HP] = 0;
	_valueMap[ATK] = 0;
	_valueMap[ISIDLE] = false;
	_valueMap[ALIVE] = true;
	_valueMap[EXPERIENCE] = 0;
	_valueMap[ISMOVABLE] = false;
	_valueMap[ISHIT] = false;
	_valueMap[ISONDEFENSE] = false;
	_valueMap[ISATTACKING] = false;
	_valueMap[MAXSTAMINA] = 0;
	_valueMap[STAMINA] = 0;
	_valueMap[ISMOVING] = false;
	_valueMap[ISROTATING] = false;
	_valueMap[ISTARGETTING] = false;
	_valueMap[TERRITORYRADIUS] = 0;
	_valueMap[ISMOVINGFORWARD] = false;
	_valueMap[ISMOVINGBACKWARD] = false;
	_valueMap[STARTATTACK] = false;
	_valueMap[ROTATINGLEFT] = true;
	_valueMap[ROTATINGRIGHT] = true;
	_valueMap[ISVERIFING] = false;
	_valueMap[STAMINALOSS] = 0;
	// use enum type to replace string
};

EntityProperties::~EntityProperties(void) {
	return;
}

cocos2d::Value EntityProperties::getValue(propertiesList val) {
	return _valueMap[val];
}

void EntityProperties::setProperties(propertiesList item, cocos2d::Value val) {
	_valueMap[item] = val;
	return;
}

void EntityProperties::addProperties(propertiesList item, cocos2d::Value val) {
	_valueMap[item] = _valueMap[item].asDouble() + val.asDouble();
	return;
}

void EntityProperties::minusProperties(propertiesList item, cocos2d::Value val) {
	_valueMap[item] = _valueMap[item].asDouble() - val.asDouble();
	return;
}
