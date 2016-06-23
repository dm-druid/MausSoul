/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by nemoremold
 */

#ifndef _NEW_SPRITE_H_
#define _NEW_SPRITE_H_

#include "cocos2d.h"
#include <string>

class NewSprite : public cocos2d::Node {
public:
	NewSprite(void);
	~NewSprite(void) = default;

	CREATE_FUNC(NewSprite);
	virtual bool init(void);

	void bindBodySprite(cocos2d::Sprite *);
	void bindLeftHandSprite(const std::string &);
	void bindRightHandSprite(const std::string &);
	void setSpriteName(const std::string &);
	cocos2d::Sprite *getBodySprite(void);
	cocos2d::Sprite *getLeftHandSprite(void);
	cocos2d::Sprite *getRightHandSprite(void);

private:
	std::string name;

	cocos2d::Sprite *_body;
	cocos2d::Sprite *_leftHand;
	cocos2d::Sprite *_rightHand;
};

#endif
