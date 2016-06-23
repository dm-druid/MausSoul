/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by SXKDZ, nemoremold, lisirrx
 */

#include "SceneOne.h"
#include "GameItemLayer.h"
#include "BackgroundLayer.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

SceneMapBase::SceneMapBase(void) {
	resetAllKey();
	return;
}

SceneMapBase::~SceneMapBase(void) {
	delete _eventHandle;
	return;
}

void SceneMapBase::resetAllKey(void) {
	_pressStateAttack = false;
	_pressStateDefend = false;
	_pressStateSprint = false;
	_pressStateUp = false;
	_pressStateDown = false;
	_pressStateLeft = false;
	_pressStateRight = false;
	_pressStateDebug = false;
	_debugMode = false;
}

void SceneMapBase::reset(float delay) {
	_player->reset();
	for (auto monster : _monsterVec) {
		if (monster->getEntityName() != "Boss") {
			monster->reset();
		}
		else if (monster->getProperties()->getValue(ALIVE).asBool()){
			monster->getSprite()->getBodySprite()->stopAllActions();
			monster->getProperties()->setProperties(ALIVE, cocos2d::Value(true));
			monster->getProperties()->setProperties(HP, monster->getProperties()->getValue(MAXHP));
			monster->getProperties()->setProperties(STAMINA, monster->getProperties()->getValue(MAXSTAMINA));
			monster->getProperties()->setProperties(TERRITORYRADIUS, cocos2d::Value(500));
			monster->setInitialCoordinate(
				monster->getProperties()->getValue(INITPOSITIONX).asDouble(),
				monster->getProperties()->getValue(INITPOSITIONY).asDouble());
			monster->setVisible(true);
			monster->setOrientation(monster->getProperties()->getValue(INITORIENT).asDouble());

			auto animation = Anim::createAnimation("res\\boss\\boss body animation\\",
				"boss", "_body", 1, 9, 0.1);
			animation->setLoops(-1);
			monster->getSprite()->getBodySprite()->runAction(cocos2d::Animate::create(animation));
		}
	}
	scheduleUpdate();
	_background->setViewPointByPlayer(_player);
	audioSetup();
	return;
}

void SceneMapBase::pushDeadScene(void) {
	unscheduleUpdate();
	resetAllKey();
	cocos2d::Director::getInstance()->pushScene(
		cocos2d::TransitionFade::create(5.0, DeadScene::create()));
	scheduleOnce(schedule_selector(SceneMapBase::reset), 5.0);
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	return;
}

bool SceneMapBase::init(void) {
	if (!Scene::init()) {
		return false;
	}

	_mainLayer = cocos2d::Layer::create();
	_mainLayer->setPosition(cocos2d::Vec2::ZERO);
	_mainLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->addChild(_mainLayer);
	sceneOneWorldEdit();
	audioSetup();
	resetAllKey();
	_gameItemLayer = GameItemLayer::create();
	this->addChild(_gameItemLayer, 10, 10);
	
	this->scheduleUpdate();	
	return true;
}

void SceneMapBase::audioSetup(void) {
	auto *audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	audioEngine->preloadBackgroundMusic("music\\background.mp3");
	audioEngine->playBackgroundMusic("music\\background.mp3", true);
	return;
}

void SceneMapBase::changeDebugMode(void) {
	_debugMode = !_debugMode;
	// if _debugMode is true, turn off all the switches
	_eventHandle->eventSwitch(EVENT_MONSTER_MARK, !_debugMode);
	_eventHandle->eventSwitch(EVENT_PASSBY_NPC, !_debugMode);
	_eventHandle->eventSwitch(EVENT_COLLISION, !_debugMode);
	_eventHandle->eventSwitch(EVENT_ATTACK, !_debugMode);
	_eventHandle->eventSwitch(EVENT_MONSTER_AUTOMOVE, !_debugMode);
	_eventHandle->eventSwitch(EVENT_MONSTER_CHASE, !_debugMode);
	_eventHandle->eventSwitch(EVENT_MONSTER_ATTACK, !_debugMode);
}

void SceneMapBase::update(float delta) {
	cocos2d::log("cocos2d::Point(%lf, %lf), %lf", _player->getPosition().x, _player->getPosition().y, _player->getOrientation());
	_background->initSurroundingBackground(_player->getPosition());
	if (_pressStateDebug) {
		changeDebugMode();
		_pressStateDebug = false;
	}
	if (_pressStateAttack) {
		if (_player->staminaEnough()) {
			_player->attack();
			_player->getProperties()->setProperties(STARTATTACK, cocos2d::Value(true));
		}
	}
	else {
		_player->getProperties()->setProperties(STARTATTACK, cocos2d::Value(false));
	}
	if (_pressStateDefend) {
		if (_player->staminaEnough()) {
			_player->defend();
		}
	}

	if (_pressStateSprint) {
		_player->sprint();
	}
	else {
		_player->moderate();
	}
	if ((_debugMode || _background->isAccessible(_player->getPosition(), _player))
		&& !_player->getProperties()->getValue(ISVERIFING).asBool()) {
		// first we need to check whether next step is available
		if (_pressStateUp) {
			_player->moveForward();
		}
		if (_pressStateDown) {
			_player->moveBackward();
		}
		if (_pressStateLeft) {
			_player->rotateLeft();
		}
		if (_pressStateRight) {
			_player->rotateRight();
		}
		_background->setViewPointByPlayer(_player);
	}
	else {
		if (_pressStateUp) {
			_player->moveBackward();
			_player->moveBackward();
			_player->sprint();
			_pressStateUp = false;
		}
		if (_pressStateDown) {
			_player->moveForward();
			_pressStateDown = false;
		}
		if (_pressStateLeft) {
			_player->rotateRight();
			_player->rotateRight();
			//_pressStateLeft = false;
		}
		if (_pressStateRight) {
			_player->rotateLeft();
			_player->rotateLeft();
			//_pressStateRight = false;
		}

	}

	if (!_player->getProperties()->getValue(ALIVE).asBool()) {
		pushDeadScene();
	}
	_player->recoverStamina();

	// for each monster, check the Events
	for (auto monster : _monsterVec) {
		if (!monster->getProperties()->getValue(ALIVE).asBool()) {
			continue; // The dead is dead
		}
		_eventHandle->runEvent(monster);
		monster->recoverStamina();
	}

	//for each NPC, check the Events
	for (auto npc : _neutralVec) {
		if (!npc->getProperties()->getValue(ALIVE).asBool()) {
			continue; // The dead is dead
		}
		_eventHandle->runEvent(npc);
	}

	for (auto substance : _SubstanceVec) {
		if (!substance->getProperties()->getValue(ALIVE).asBool()) {
			continue; // The dead is dead
		}
		_eventHandle->runEvent(substance);
	}
	_gameItemLayer->setHP(_player->getPercentHP());
	_gameItemLayer->setStamina(_player->getPercentStamina());
	_gameItemLayer->setExp(_player->getProperties()->getValue(EXPERIENCE).asInt());
	return;
}

void SceneMapBase::sceneOneWorldEdit(void) {
	addMap();
	addPlayer();
	addEvent();
	addNeutral(cocos2d::Point(3878.230713f, 2658.114258f), 65);
	addBoss(cocos2d::Point(5030.411621f, 3834.959229f), 220);
	addBoss(cocos2d::Point(1566.276978f, 2099.577393f), 245);

	addMonster(cocos2d::Point(3090.139893f, 3541.892334f), 135);
	addMonster(cocos2d::Point(4412.496582f, 4587.584961f), 82);
	addMonster(cocos2d::Point(6834.441406f, 3199.499023f), 350);
	addMonster(cocos2d::Point(6095.868164f, 1052.717773f), 270);
	addMonster(cocos2d::Point(5823.812500f, 1346.547119f), 0);
	addMonster(cocos2d::Point(5265.115234f, 1756.813843f), 130);
	addMonster(cocos2d::Point(3533.618408f, 1087.630005f), 0);
	addMonster(cocos2d::Point(3065.290527f, 1945.203491f), 200);
	addMonster(cocos2d::Point(2304.357422f, 3890.448975f), 80);
	addMonster(cocos2d::Point(1361.956421f, 3958.631592f), 100);

	std::vector<cocos2d::Point> points = { cocos2d::Point(4446.850098f, 2281.639404f),  cocos2d::Point(4051.495850f, 3374.257568f),
		cocos2d::Point(4332.202148f, 3269.864014f), cocos2d::Point(3503.357910f, 4280.112793f),
		cocos2d::Point(6026.457520f, 889.121887f), cocos2d::Point(3393.815674f, 2024.070679f),
		cocos2d::Point(1843.145264f, 4645.825684f) };
	std::vector<double> orients = { 305, 280, 305, 270, 95, 310, 270 };

	for (unsigned i = 0; i < points.size(); ++i) {
		if (i == 6) {
			addSubstance(TREASURECHEST, points.at(i), orients.at(i),true);
		}
		else {
			addSubstance(TREASURECHEST, points.at(i), orients.at(i));
		}
	}

	addSubstance(DESK, cocos2d::Point(3811.171143f, 2814.161621f), 160);
	addSubstance(DESK, cocos2d::Point(4129.828125f, 2713.689209f), 30);
	addSubstance(FIRECHAIR, cocos2d::Point(3731.510742f, 2664.234619f), 20);
	addSubstance(FIRECHAIR, cocos2d::Point(4047.275391f, 2636.591797f), 345);
	addSubstance(FIRECHAIR, cocos2d::Point(4300.189941f, 2723.288086f), 260);
	addSubstance(FIRECHAIR, cocos2d::Point(4019.791260f, 2849.504883f), 165);
	addSubstance(FIRECHAIR, cocos2d::Point(3468.662354f, 3594.926514f), 285);
	addSubstance(FIRECHAIR, cocos2d::Point(3499.262451f, 3731.652100f), 265);
	addSubstance(FIRECHAIR, cocos2d::Point(3330.811768f, 3970.795654f), 205);
	addSubstance(BOX1, cocos2d::Point(4593.632324f, 2911.979004f), 20);
	addSubstance(BOX1, cocos2d::Point(4305.739746f, 3135.760254f), 190);
	addSubstance(BOX1, cocos2d::Point(4182.397461f, 2342.080322f), 225);
	addSubstance(BOX1, cocos2d::Point(3545.210693f, 2207.970459f), 195);
	addSubstance(BOX1, cocos2d::Point(3669.415527f, 2115.909912f), 185);
	addSubstance(BOX1, cocos2d::Point(4204.688477f, 2038.055054f), 200);
	addSubstance(BOX1, cocos2d::Point(3775.589600f, 3249.795654f), 10);
	addSubstance(BOX1, cocos2d::Point(5976.866699f, 2617.050537f), 10);
	addSubstance(BOX1, cocos2d::Point(5854.373047f, 2500.737793f), 345);
	addSubstance(BOX1, cocos2d::Point(6327.042480f, 3574.481445f), 200);
	addSubstance(BOX1, cocos2d::Point(6752.988281f, 2117.748535f), 20);
	addSubstance(BOX1, cocos2d::Point(6778.457031f, 1940.997803f), 75);
	addSubstance(BOX1, cocos2d::Point(5880.671387f, 940.120544f), 335);
	addSubstance(BOX1, cocos2d::Point(3535.493652f, 1919.045898f), 330);
	addSubstance(BOX1, cocos2d::Point(2096.343506f, 2456.305908f), 335);
	addSubstance(BOX1, cocos2d::Point(1993.915771f, 821.654724f), 200);
	addSubstance(BOX1, cocos2d::Point(1871.021362f, 795.478210f), 350);
	addSubstance(BOX1, cocos2d::Point(1960.017456f, 925.743774f), 240);
	addSubstance(BOX2, cocos2d::Point(4155.510742f, 2454.498779f), 205);
	addSubstance(BOX2, cocos2d::Point(3924.034668f, 3352.268555f), 255);
	addSubstance(BOX2, cocos2d::Point(4001.600830f, 3254.801514f), 230);
	addSubstance(BOX2, cocos2d::Point(4634.088867f, 2786.196777f), 340);
	addSubstance(BOX2, cocos2d::Point(4636.624023f, 2674.259277f), 270);
	addSubstance(BOX2, cocos2d::Point(3280.505371f, 2731.443359f), 195);
	addSubstance(BOX2, cocos2d::Point(3269.532227f, 2562.661133f), 165);
	addSubstance(BOX2, cocos2d::Point(3574.722656f, 2093.651611f), 355);
	addSubstance(BOX2, cocos2d::Point(3701.448486f, 1998.155396f), 195);
	addSubstance(BOX2, cocos2d::Point(3680.610840f, 3191.807861f), 340);
	addSubstance(BOX2, cocos2d::Point(3374.617432f, 4251.655273f), 350);
	addSubstance(BOX2, cocos2d::Point(6270.478027f, 3678.457275f), 135);
	addSubstance(BOX2, cocos2d::Point(6682.568359f, 2190.705078f), 125);
	addSubstance(BOX2, cocos2d::Point(5024.466309f, 1928.067505f), 230);
	addSubstance(BOX2, cocos2d::Point(4973.232422f, 1766.674194f), 180);
	addSubstance(BOX2, cocos2d::Point(4066.419922f, 1284.837036f), 245);
	addSubstance(BOX2, cocos2d::Point(3415.638916f, 1890.546631f), 265);
	addSubstance(BOX2, cocos2d::Point(3288.802002f, 2134.018799f), 250);
	addSubstance(BOX2, cocos2d::Point(1220.367188f, 2230.793457f), 115);
	addSubstance(BOX2, cocos2d::Point(1172.523560f, 1634.818115f), 210);
	addSubstance(BOX2, cocos2d::Point(2169.816406f, 1911.233887f), 15);
	addSubstance(BOX2, cocos2d::Point(2071.848633f, 2335.605713f), 275);
	addSubstance(BOX2, cocos2d::Point(1989.433228f, 2415.208984f), 200);
	addSubstance(BOX2, cocos2d::Point(1983.314087f, 2538.935059f), 210);
	addSubstance(BOX2, cocos2d::Point(1857.301147f, 2546.872314f), 110);
	addSubstance(BARREL1, cocos2d::Point(4268.775391f, 2424.472168f), 225);
	addSubstance(BARREL1, cocos2d::Point(4351.065430f, 2517.407959f), 65);
	addSubstance(BARREL1, cocos2d::Point(4101.193359f, 3275.753662f), 320);
	addSubstance(BARREL1, cocos2d::Point(4154.512207f, 3181.257813f), 240);
	addSubstance(BARREL1, cocos2d::Point(4530.557129f, 2797.878662f), 55);
	addSubstance(BARREL1, cocos2d::Point(4656.020020f, 2580.700439f), 265);
	addSubstance(BARREL1, cocos2d::Point(4539.459473f, 2602.405762f), 270);
	addSubstance(BARREL1, cocos2d::Point(4314.270508f, 2050.171875f), 260);
	addSubstance(BARREL1, cocos2d::Point(4385.661133f, 2138.264648f), 45);
	addSubstance(BARREL1, cocos2d::Point(4323.856445f, 2260.285889f), 75);
	addSubstance(BARREL1, cocos2d::Point(3846.380615f, 4020.631348f), 55);
	addSubstance(BARREL1, cocos2d::Point(6373.972168f, 3748.278076f), 60);
	addSubstance(BARREL1, cocos2d::Point(6331.813477f, 2300.967529f), 300);
	addSubstance(BARREL1, cocos2d::Point(6217.443848f, 2191.368896f), 160);
	addSubstance(BARREL1, cocos2d::Point(6231.054688f, 942.839539f), 30);
	addSubstance(BARREL1, cocos2d::Point(6291.815918f, 1048.500977f), 110);
	addSubstance(BARREL1, cocos2d::Point(3959.009766f, 1251.133789f), 125);
	addSubstance(BARREL1, cocos2d::Point(925.309021f, 2197.750488f), 190);
	addSubstance(BARREL1, cocos2d::Point(1002.317810f, 2039.391235f), 105);
	addSubstance(BARREL1, cocos2d::Point(2182.610352f, 2319.751465f), 50);
	addSubstance(BARREL1, cocos2d::Point(2156.969971f, 1802.309448f), 55);
	addSubstance(BARREL1, cocos2d::Point(1866.518677f, 910.335083f), 285);
	addSubstance(BARREL2, cocos2d::Point(4201.771484f, 3325.657959f), 80);
	addSubstance(BARREL2, cocos2d::Point(4431.859375f, 3176.922119f), 135);
	addSubstance(BARREL2, cocos2d::Point(4631.693848f, 2494.189209f), 115);
	addSubstance(BARREL2, cocos2d::Point(3839.380859f, 3116.228516f), 160);
	addSubstance(BARREL2, cocos2d::Point(3707.266602f, 3055.200928f), 135);
	addSubstance(BARREL2, cocos2d::Point(4258.132813f, 2521.952148f), 55);
	addSubstance(BARREL2, cocos2d::Point(4385.957031f, 2603.697998f), 35);
	addSubstance(BARREL2, cocos2d::Point(4477.678711f, 2164.028564f), 135);
	addSubstance(BARREL2, cocos2d::Point(3874.214111f, 3927.960938f), 285);
	addSubstance(BARREL2, cocos2d::Point(6418.391602f, 3839.839600f), 45);
	addSubstance(BARREL2, cocos2d::Point(6459.624512f, 3714.072754f), 85);
	addSubstance(BARREL2, cocos2d::Point(6641.518066f, 2293.593750f), 180);
	addSubstance(BARREL2, cocos2d::Point(6232.489746f, 2287.355957f), 135);
	addSubstance(BARREL2, cocos2d::Point(3220.969482f, 2051.364746f), 60);
	addSubstance(BARREL2, cocos2d::Point(3176.181152f, 2169.213623f), 275);
	addSubstance(BARREL2, cocos2d::Point(1900.624023f, 2634.656250f), 45);
	addSubstance(BARREL2, cocos2d::Point(1030.269897f, 2145.170898f), 175);
	addSubstance(BARREL2, cocos2d::Point(1897.500732f, 1007.572876f), 15);
	addSubstance(REVIVALSTONE, cocos2d::Point(1830, 4090), 0);
}

/*
* beta version of adding of substance
* codes need improving pressingly
* another matter is that how to number each substance since they are not stored seperatedly
*/
void SceneMapBase::addSubstance(SubstanceType type, cocos2d::Point pos, double orient, bool flag) {
	if (type == TREASURECHEST) {
		Substance *chest = Substance::create();
		auto sprite1 = cocos2d::Sprite::create("res\\objects\\chest.png");
		auto chestSprite1 = NewSprite::create();
		chestSprite1->bindBodySprite(sprite1);
		auto chestSprite2 = cocos2d::Sprite::create("res\\objects\\chest_open.png");

		chest->setEntityName("Chest");
		chest->setBody(Body{ { 0 },{ 0 }, 70 });
		chest->setSubstanceType(TREASURECHEST);

		auto content1 = new Equip;
		if (flag) {
			content1->setAddValue(ADDATK, cocos2d::Value(100.0));
			content1->setEquipType(WEAPON);
			content1->setAttackArea({ { cocos2d::Vec2::ZERO },{ 200 },{ 60 } });
			content1->setName("advancedequip");
			chest->bindOneTimeEquip(content1);
		}
		else {
			content1->setAddOnce(ADDONCEHP, cocos2d::Value(100));
			chest->bindOneTimeEquip(content1);
		}
		_SubstanceVec.push_back(chest);
		_mainLayer->addChild(chest);
		// pay attention to the order 
		chest->bindSprite(chestSprite1);
		chest->setInitialCoordinate(pos.x, pos.y);
		chest->bindSprite2nd(chestSprite2);
		chestSprite2->setRotation(orient);
		chest->setOrientation(orient);
	}
	else if (type == REVIVALSTONE) {
		auto revivalStone = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\barrel1.png");
		auto stoneSprite1 = NewSprite::create();
		stoneSprite1->bindBodySprite(sprite);
		auto stoneSprite2 = cocos2d::Sprite::create("res\\objects\\barrel2.png");

		revivalStone->setEntityName("revivalStone");
		revivalStone->setBody(Body{ { 0 },{ 0 }, 45 });
		revivalStone->setSubstanceType(REVIVALSTONE);
		//revivalStone->setBreakable(true);

		_SubstanceVec.push_back(revivalStone);
		_mainLayer->addChild(revivalStone);

		revivalStone->bindSprite(stoneSprite1);
		revivalStone->setInitialCoordinate(pos.x, pos.y);
		//revivalStone->bindSprite2nd(stoneSprite2);
		revivalStone->setOrientation(orient);
	}
	else if (type == DESK) {
		auto desk = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\desk.png");
		auto deskSprite = NewSprite::create();
		deskSprite->bindBodySprite(sprite);

		_SubstanceVec.push_back(desk);
		_mainLayer->addChild(desk);

		desk->bindSprite(deskSprite);
		desk->setInitialCoordinate(pos.x, pos.y);
		desk->setOrientation(orient);
		desk->setSubstanceType(DESK);
		desk->setBody(Body{ { 0 },{ 0 }, 80 });
	}
	else if (type == BOX1) {
		auto box = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\box1.png");
		auto boxSprite = NewSprite::create();
		boxSprite->bindBodySprite(sprite);

		_SubstanceVec.push_back(box);
		_mainLayer->addChild(box);

		box->bindSprite(boxSprite);
		box->setInitialCoordinate(pos.x, pos.y);
		box->setOrientation(orient);
		box->setSubstanceType(BOX1);
		box->setBody(Body{ { 0 },{ 0 }, 50 });

	}
	else if (type == BOX2) {
		auto box = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\box2.png");
		auto boxSprite = NewSprite::create();
		boxSprite->bindBodySprite(sprite);

		_SubstanceVec.push_back(box);
		_mainLayer->addChild(box);

		box->bindSprite(boxSprite);
		box->setInitialCoordinate(pos.x, pos.y);
		box->setOrientation(orient);
		box->setSubstanceType(BOX2);
		box->setBody(Body{ { 0 },{ 0 }, 40 });

	}
	else if (type == BARREL1) {
		auto barrel = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\barrel1.png");
		auto barrelSprite = NewSprite::create();
		barrelSprite->bindBodySprite(sprite);

		_SubstanceVec.push_back(barrel);
		_mainLayer->addChild(barrel);

		barrel->bindSprite(barrelSprite);
		barrel->setInitialCoordinate(pos.x, pos.y);
		barrel->setOrientation(orient);
		barrel->setSubstanceType(BARREL1);
		barrel->setBody(Body{ { 0 },{ 0 }, 45 });
	}
	else if (type == BARREL2) {
		auto barrel = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\barrel2.png");
		auto barrelSprite = NewSprite::create();
		barrelSprite->bindBodySprite(sprite);

		_SubstanceVec.push_back(barrel);
		_mainLayer->addChild(barrel);

		barrel->bindSprite(barrelSprite);
		barrel->setInitialCoordinate(pos.x, pos.y);
		barrel->setOrientation(orient);
		barrel->setSubstanceType(BARREL2);
		barrel->setBody(Body{ { 0 },{ 0 }, 45 });

	}
	else if (type == FIRECHAIR) {
		auto chair = Substance::create();
		auto sprite = cocos2d::Sprite::create("res\\objects\\chair.png");
		auto chairSprite = NewSprite::create();
		chairSprite->bindBodySprite(sprite);

		_SubstanceVec.push_back(chair);
		_mainLayer->addChild(chair);

		chair->bindSprite(chairSprite);
		chair->setInitialCoordinate(pos.x, pos.y);
		chair->setOrientation(orient);
		chair->setSubstanceType(FIRECHAIR);
		chair->setBody(Body{ { 0 },{ 0 }, 40 });
	}
	/*auto centerTip = cocos2d::Sprite::create("res\\neutral\\tip.png");
	auto substance = *(_SubstanceVec.cbegin());
	centerTip->setPosition(substance->getPosition());
	substance->addChild(centerTip);*/
}

void SceneMapBase::addNeutral(cocos2d::Point pos, double orient) {
	NPC* neutral = NPC::create();
	auto neutralSprite = NewSprite::create();
	auto *sprite = cocos2d::Sprite::create("res\\neutral\\rat.png");
	neutralSprite->bindBodySprite(sprite);
	neutralSprite->bindLeftHandSprite("res\\neutral\\left hand\\firestartpos.png");
	neutralSprite->bindRightHandSprite("res\\neutral\\right hand\\firestartpos.png");
	sprite->setColor(cocos2d::Color3B(50, 50, 200));

	char buffer[10] = { 0 };
	sprintf(buffer, "_%d", _neutralVec.size());
	std::string entityName = (std::string)"Monster" + buffer;
	neutral->setEntityName(entityName);
	neutralSprite->setSpriteName(entityName);

	neutral->bindSprite(neutralSprite);
	neutral->setInitialCoordinate(pos.x, pos.y);
	neutral->setOrientation(orient);
	neutral->addDialogue("res\\neutral\\tip.png");
	// Once the pictures' size changed, raplace the 100 with its contantSize
	neutral->setBody(Body{ angle, radius, 90.0 });

	auto animation = Anim::createAnimation("res\\neutral\\rat body animation\\",
		"rat", "_animation", 1, 9, 0.1);
	animation->setLoops(-1);
	neutral->getSprite()->getBodySprite()->runAction(cocos2d::Animate::create(animation));

	// add the NPC to the NPC manager
	_neutralVec.push_back(neutral);
	// add the NPC to the main layer
	_mainLayer->addChild(neutral, 1);
	return;
}


void SceneMapBase::addEvent(void) {
	EventHandle::registerEventHandle(_player);
	_eventHandle = new EventHandle;
	// delete it when change the scene?
	_eventHandle->eventSwitch(EVENT_MONSTER_MARK, true);
	return;
}

void SceneMapBase::setKeyPressed(Key key) {
	switch (key) {
	case SceneMapBase::Key::UP:
		_pressStateUp = false;
		break;
	case SceneMapBase::Key::DOWN:
		_pressStateDown = false;
		break;
	case SceneMapBase::Key::LEFT:
		_pressStateLeft = true;
		break;
	case SceneMapBase::Key::RIGHT:
		_pressStateRight = true;
		break;
	}
}

void SceneMapBase::addMap(void) {
	/*
	 * maps are placed from the down-left corner of the main layer,
	 * their anchor point shares the same solution of the main layer
	 */
	_background = BackgroundLayer::create(
		"map\\cover1.%c.%r.png",
		"map\\mask1.%c.%r.png",
		1, 5,
		1, 4);
	_background->setAnchorPoint(cocos2d::Vec2::ZERO);
	_background->setPosition(cocos2d::Vec2::ZERO);
	_background->setName("background");
	_mainLayer->addChild(_background, 0);
	return;
}

void SceneMapBase::addPlayer(void) {
	auto player = Player::create();
	auto playerSprite = NewSprite::create();
	_player = player;
	_player->setEntityName("player");
	playerSprite->setSpriteName("player");

	auto *sprite = cocos2d::Sprite::create("res\\player\\rat.png");
	playerSprite->bindBodySprite(sprite);
	playerSprite->bindLeftHandSprite("res\\player\\left hand\\firestartpos.png");
	playerSprite->bindRightHandSprite("res\\player\\right hand\\firestartpos.png");
	auto size = sprite->getContentSize();

	player->bindSprite(playerSprite);
	player->setSpeed(3);
	player->setAcceleration(0.1);
	player->setAngularSpeed(5);
	player->setOrientation(270);
	player->setInitialCoordinate(3916.689697f, 2359.860352f);
	_background->initSurroundingBackground(_player->getPosition());

	player->setBody(Body{ angle, radius, 90.0 });
	player->getProperties()->setProperties(ATK, cocos2d::Value(250.0));
	player->getProperties()->setProperties(MAXHP, cocos2d::Value(1000.0));
	player->getProperties()->setProperties(HP, cocos2d::Value(1000.0));
	player->getProperties()->setProperties(STAMINA, cocos2d::Value(1000.0));
	player->getProperties()->setProperties(MAXSTAMINA, cocos2d::Value(1000.0));
	player->getProperties()->setProperties(RECOVERSTAMINA, cocos2d::Value(5.0));
	player->getProperties()->setProperties(STAMINALOSS, cocos2d::Value(300.0));

	_mainLayer->addChild(player, 1);

	auto animation = Anim::createAnimation("res\\player\\rat body animation\\",
		"rat", "_animation", 1, 9, 0.1);
	animation->setLoops(-1);
	player->getSprite()->getBodySprite()->runAction(cocos2d::Animate::create(animation));
	_background->setViewPointByPlayer(_player);

	auto keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(SceneMapBase::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(SceneMapBase::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, player);
	return;
}

/*
 * NOTICE:
 * the solution of the monster may not like the player, though they derived from the same base class
 * this is the beta version of monster editor
 * time allows, methods may change
 * suitable for beta version of the game
 */

void SceneMapBase::addBoss(cocos2d::Point pos, double orient) {
	auto boss = Monster::create();
	auto bossSprite = NewSprite::create();
	auto sprite = cocos2d::Sprite::create("res\\boss\\boss.png");
	bossSprite->bindBodySprite(sprite);
	bossSprite->bindLeftHandSprite("res\\boss\\attack type 1\\lightstartpos.png");
	bossSprite->bindRightHandSprite("res\\boss\\attack type 2\\lightstartpos.png");
	boss->bindSprite(bossSprite);

	boss->setEntityName("Boss");
	bossSprite->setSpriteName("Boss");

	boss->setInitialCoordinate(pos.x, pos.y);
	boss->setOrientation(orient);
	boss->setBody(Body{ angle, radius, 120.0 });
	boss->setAttackArea({ { cocos2d::Vec2::ZERO },{ 200.0 },{ 60.0 } });

	boss->getProperties()->setProperties(MAXHP, cocos2d::Value(2800.0));
	boss->getProperties()->setProperties(HP, cocos2d::Value(2800.0));
	boss->getProperties()->setProperties(ATK, cocos2d::Value(300));
	boss->getProperties()->setProperties(TERRITORYRADIUS, cocos2d::Value(500));
	boss->setSpeed(2.6);
	boss->setAcceleration(0.1);
	boss->getProperties()->setProperties(EXPERIENCE, cocos2d::Value(2000));

	auto animation = Anim::createAnimation("res\\boss\\boss body animation\\",
		"boss", "_body", 1, 9, 0.1);
	animation->setLoops(-1);
	boss->getSprite()->getBodySprite()->runAction(cocos2d::Animate::create(animation));

	_monsterVec.push_back(boss);
	// add the boss to the main layer
	_mainLayer->addChild(boss, 1);
}

void SceneMapBase::addMonster(cocos2d::Point pos, double orient) {
	auto monster = Monster::create();
	auto monsterSprite = NewSprite::create();
	auto *sprite = cocos2d::Sprite::create("res\\monster\\rat.png");
	monsterSprite->bindBodySprite(sprite);
	monsterSprite->bindLeftHandSprite("res\\monster\\left hand\\firestartpos.png");
	monsterSprite->bindRightHandSprite("res\\monster\\right hand\\firestartpos.png");
	monster->bindSprite(monsterSprite);

	char buffer[10] = { 0 };
	sprintf(buffer, "_%d", _monsterVec.size());
	std::string entityName = (std::string)"Monster" + buffer;
	monster->setEntityName(entityName);
	monsterSprite->setSpriteName(entityName);

	monster->setInitialCoordinate(pos.x, pos.y);
	monster->setOrientation(orient);
	monster->setBody(Body{ angle, radius, 90.0 });
	// You can set the size of the monster
	monster->setAttackArea({ { cocos2d::Vec2::ZERO },{ 200.0 },{ 60.0 } });

	monster->getProperties()->setProperties(MAXHP, cocos2d::Value(500.0));
	monster->getProperties()->setProperties(HP, cocos2d::Value(500.0));
	monster->getProperties()->setProperties(ATK, cocos2d::Value(200.0));
	monster->setSpeed(2.6);
	monster->setAcceleration(0.1);
	monster->getProperties()->setProperties(EXPERIENCE, cocos2d::Value(100));
	monster->getProperties()->setProperties(INITORIENT, cocos2d::Value(orient));

	auto animation = Anim::createAnimation("res\\monster\\monster body animation\\",
		"rat", "_animation", 1, 9, 0.1);
	animation->setLoops(-1);
	monster->getSprite()->getBodySprite()->runAction(cocos2d::Animate::create(animation));

	_monsterVec.push_back(monster);
	// add the monsters to the main layer
	_mainLayer->addChild(monster, 1);
}

void SceneMapBase::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
	switch (keyCode) {
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
		_pressStateUp = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		_pressStateDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
		_pressStateLeft = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		_pressStateRight = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_J:
		_pressStateAttack = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_K:
		_pressStateDefend = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CTRL:
	case cocos2d::EventKeyboard::KeyCode::KEY_SHIFT:
		_pressStateSprint = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_0:
		_pressStateDebug = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_Q:
		if (_player->getEquipMaxnum()) {
			_player->changeEquips();
		}
	}
	return;
}

void SceneMapBase::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
	switch (keyCode) {
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
		_pressStateUp = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		_pressStateDown = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
		_pressStateLeft = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		_pressStateRight = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_J:
		_pressStateAttack = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_K:
		_pressStateDefend = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_CTRL:
	case cocos2d::EventKeyboard::KeyCode::KEY_SHIFT:
		_pressStateSprint = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_0:
		_pressStateDebug = false;
		break;
	default:
		break;
	}
} 
