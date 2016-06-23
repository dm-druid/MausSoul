/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM, amplified by nemoremold
 */

#ifndef _ENTITY_PROPERTIES_H_
#define _ENTITY_PROPERTIES_H_

#include "cocos2d.h"
#include <map>
#include <string>

constexpr int MAX_VELOCITY = 6;

// list of properties the Property System stores
enum propertiesList : int { INITPOSITIONX, INITPOSITIONY, DEFAULTVELOCITY, VELOCITY, MAXHP,
							ACCELERATION, ANGULARVELOCITY, ORIENTATION, ATK, DEFENCE, HP, ISIDLE, ALIVE, EXPERIENCE,
							ISMOVABLE, ISHIT, ISONDEFENSE, ISATTACKING, ISMOVING, ISROTATING, ISTARGETTING,
							TERRITORYRADIUS, ISMOVINGFORWARD, ISMOVINGBACKWARD, STARTATTACK, ISVERIFING, ROTATINGLEFT,
							ROTATINGRIGHT, STAMINA, MAXSTAMINA, RECOVERSTAMINA, STAMINALOSS, INITORIENT};

class EntityProperties {
public:
	EntityProperties(void);
	~EntityProperties(void);
	void setProperties(propertiesList, cocos2d::Value);
	void addProperties(propertiesList, cocos2d::Value);
	void minusProperties(propertiesList, cocos2d::Value);
	
	cocos2d::Value getValue(propertiesList); // get Value and transform by yourself
private:
	// using Value type defined in cocos2d itself for covenient storing of values;
	std::map<propertiesList , cocos2d::Value> _valueMap; // change?
};

#endif /* _ENTITY_PROPERTIES_H_ */
