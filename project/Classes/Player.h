/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Body.h"
#include "Entity.h"
#include "cocos2d.h"

class BackgroundLayer;

class Player : public Entity {
public:
	Player(void);
	~Player(void) = default;

	CREATE_FUNC(Player);
	virtual bool init(void);

	virtual void attack(void);
	virtual void defend(void);
	void hit(double atk);
	virtual void die(void);
	virtual void reset(void);
	void checkHP(void);


	double getPercentHP(void);
	double getPercentStamina(void);
};

#endif
