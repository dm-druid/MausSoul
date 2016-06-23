/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#ifndef _SUBSTANCE_H_
#define _SUBSTANCE_H_

#include "Entity.h"
#include "Equip.h"

enum SubstanceType : int { BOX1, BOX2, BARREL1, BARREL2, DESK, FIRECHAIR, TREASURECHEST, REVIVALSTONE };

constexpr int TIMES_TO_BE_BROKEN = 1;

class Substance:public Entity{
public:
	Substance(void) = default;
	~Substance(void);
	CREATE_FUNC(Substance);
	virtual bool init(void);
	void bindSprite2nd(cocos2d::Sprite *);
	void changeState(void);
	void bindOneTimeEquip(Equip *); // bind a one-time equipment
	void setSubstanceType(SubstanceType);
	SubstanceType getSubstanceType(void);
	Equip *getContent(void);
	void unhit(float delay);
	bool hit(void);
	void destroy(void);
	bool getFirstOpen(void);
private:
	SubstanceType _substanceType;
	cocos2d::Sprite *_sprite2nd; // restore the seccond state
	int _state;
	bool _firstOpen;
	// assume that a box (or something) needs several times of attacking to be broken
	int _timesToBeBroken; 
	Equip* _content; // temporarily use the equip class as the content
};

#endif
