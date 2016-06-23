/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by lisirrx
 */

#ifndef _STARTSCENE_H_
#define _STARTSCENE_H_

#include "SceneOne.h"
#include "cocos2d.h"

class StartScene : public cocos2d::Scene {
public:
	StartScene(void) : m_numSp(20), m_loadedSp(0) {};

	virtual bool init(void);
	void loadingCallback(cocos2d::Texture2D* pSender);

	void addReasources(Ref *ref);
	void exitGame(cocos2d::Ref *ref);
	CREATE_FUNC(StartScene);

private:
	cocos2d::Label *title;
	cocos2d::MenuItemLabel *begin;
	cocos2d::MenuItemLabel *end;
	int m_numSp;
	int m_loadedSp;
};

#endif