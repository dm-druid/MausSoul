/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM, nemoremold, lisirrx
 */

#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "cocos2d.h" 
#include "Entity.h"

class Monster : public Entity {
public:
	Monster(void);
	~Monster(void) = default;

	CREATE_FUNC(Monster);
	virtual bool init(void);
	bool hit(double atk);
	void attack(void);
	void defend(void);
	void die(void);
	virtual void reset(void);
	void automove(void); // used for tests, making the monster to move automatically
	void resetPos(void);
};

#endif
