/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#ifndef _DEADSCENE_H_
#define _DEADSCENE_H_

#include "cocos2d.h"

class DeadScene : public cocos2d::Scene {
public:
	DeadScene(void) = default;
	~DeadScene(void) = default;
	virtual bool init(void);
	void audioSetup(void);
	CREATE_FUNC(DeadScene);
	static cocos2d::Scene *createScene(void);
	void popScene(/*float delay*/Ref* pSender);
private:
	cocos2d::Label *label;
};

#endif