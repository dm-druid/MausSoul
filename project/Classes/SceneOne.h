/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 * 
 * written by SXKDZ, nemoremold
 */

#ifndef _SCENEONE_H_
#define _SCENEONE_H_

#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "NewSprite.h"
#include "Substance.h"
#include "DeadScene.h"
#include "EventHandle.h"
#include "GameItemLayer.h"
#include "BackgroundLayer.h"
#include "cocos2d.h"
#include <string>

class SceneMapBase : public cocos2d::Scene {
public:
	enum Key{UP, DOWN, LEFT, RIGHT};
	SceneMapBase(void);
	~SceneMapBase(void);

	static cocos2d::Scene *createScene(void);
	virtual bool init(void) override;
	CREATE_FUNC(SceneMapBase);

	void sceneOneWorldEdit(void);
	void audioSetup(void);
	void addPlayer(void);
	void addBoss(cocos2d::Point, double);
	void addMonster(cocos2d::Point, double);
	void addNeutral(cocos2d::Point, double);
	void addMap(void);
	void addEvent(void);
	void addSubstance(SubstanceType, cocos2d::Point, double, bool flag = false);
	void setKeyPressed(Key key);
	void resetAllKey();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	virtual void update(float delta) override;
	void pushDeadScene(void);
	void reset(float delay);
	void changeDebugMode(void);

private:
	cocos2d::Layer *_mainLayer;
	GameItemLayer *_gameItemLayer;
	Player *_player;
	BackgroundLayer *_background;
	EventHandle *_eventHandle;
	std::vector<Monster *> _monsterVec; // monster manager
	std::vector<NPC *> _neutralVec; // NPC manager
	std::vector<Substance *> _SubstanceVec; // Object manager

	bool _debugMode;
	/*
	* update of the control method:
	*  each state represents the state of one key;
	*  every time update method is run, their states will be tested, and events will be handled;
	*/
	bool _pressStateAttack;
	bool _pressStateDefend;
	bool _pressStateSprint;
	bool _pressStateUp;
	bool _pressStateDown;
	bool _pressStateLeft;
	bool _pressStateRight;
	bool _pressStateDebug;
};

#endif
