/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM, nemoremold, lisirrx
 */

#include "Monster.h"

constexpr double INIT_TERRITORY_RADIUS = 300.0;

Monster::Monster(void) {
	setEntityType(MONSTER);
	_properties->setProperties(STAMINA, cocos2d::Value(1000.0));
	_properties->setProperties(MAXSTAMINA, cocos2d::Value(1000.0));
	_properties->setProperties(STAMINALOSS,cocos2d::Value(300.0));
	_properties->setProperties(RECOVERSTAMINA, cocos2d::Value(3));
	_properties->setProperties(EXPERIENCE, cocos2d::Value(100));
	return;
}

void Monster::resetPos(void) {
	setPosition(_properties->getValue(INITPOSITIONX).asDouble(),
		_properties->getValue(INITPOSITIONY).asDouble());
	return;
}

void Monster::die(void) {
	stopMovement();
	_properties->setProperties(ALIVE, cocos2d::Value(false));
	auto animation = Anim::createAnimation("res\\monster\\death animation\\",
		"monster", "_death", 1, 14, 0.05, false);
	_sprite->getBodySprite()->runAction(cocos2d::Animate::create(animation));
	return;
}

bool Monster::hit(double atk) {
	_properties->setProperties(ISHIT, cocos2d::Value(true));
	_properties->minusProperties(HP, cocos2d::Value(atk));
	cocos2d::log("%lf", _properties->getValue(HP).asDouble());
	// decide the specific position later
	auto animation = cocos2d::MoveBy::create(1.0, cocos2d::Vec2(0, 0));
	// use animation of the hitted monster to replace it.
	auto callFunc = cocos2d::CallFunc::create([&]() {
		_properties->setProperties(ISHIT, cocos2d::Value(false));
		if (getProperties()->getValue(HP).asDouble() <= 0.0) {
			die();
		}
	});
	auto actions = cocos2d::Sequence::create(animation, callFunc, nullptr);
	runAction(actions);
	if (getProperties()->getValue(HP).asDouble() <= 0.0) {
		return true;
	}
	return false;
}

bool Monster::init(void) {
	resetPos();
	_properties->setProperties(TERRITORYRADIUS, cocos2d::Value(INIT_TERRITORY_RADIUS));
	// set monsters' default terrirory radius
	return true;
}

void Monster::automove(void) {
	_properties->setProperties(ISMOVING, cocos2d::Value(true));
	auto moveby1 = cocos2d::MoveBy::create(2, cocos2d::Vec2(50, 0));
	auto moveby2 = cocos2d::MoveBy::create(2, cocos2d::Vec2(0, 50));
	auto moveby3 = cocos2d::MoveBy::create(2, cocos2d::Vec2(-50, 0));
	auto moveby4 = cocos2d::MoveBy::create(2, cocos2d::Vec2(0, -50));
	auto callFunc = cocos2d::CallFunc::create([&]() {
		_properties->setProperties(ISMOVING, cocos2d::Value(false));
	});
	auto actionsequence = cocos2d::Sequence::create(
		moveby1, moveby2 , moveby3, moveby4, callFunc, nullptr);
	this->runAction(actionsequence);
	return;
}

void Monster::attack() {
	if (_properties->getValue(ISATTACKING).asBool()) {
		return;
	}
	minusStamina();
	cocos2d::Animation *animation;
	if (getEntityName() != "Boss") {
		animation = Anim::createAnimation("res\\monster\\right hand\\", "fire", "_attack", 1, 16, 0.05);
	}
	else {
		int randomNumber = rand() % 3;
		switch (randomNumber) {
		case 0:
			animation = Anim::createAnimation("res\\monster\\right hand\\", "fire", "_attack", 1, 16, 0.05);
			setAttackArea({ { cocos2d::Vec2::ZERO },{ 200.0 },{ 60.0 } });
			break;
		case 1:
			animation = Anim::createAnimation("res\\boss\\attack type 1\\", "boss", "_attacktype1", 1, 17, 0.05);
			setAttackArea({ { cocos2d::Vec2::ZERO },{ 230.0},{ 120.0 } });
			break;
		case 2:
			animation = Anim::createAnimation("res\\boss\\attack type 2\\", "boss", "_attack2", 1, 24, 0.05);
			setAttackArea({ { cocos2d::Vec2::ZERO },{ 180.0 },{ 360.0 } });
			break;
		default:
			break;
		}
	}
	auto callfunc = cocos2d::CallFunc::create([&]() {
		_properties->setProperties(ISATTACKING, cocos2d::Value(false));
	});
	_sprite->getRightHandSprite()->stopAllActions();
	_sprite->getRightHandSprite()->runAction(
		cocos2d::Sequence::create(cocos2d::Animate::create(animation), callfunc, nullptr));
	_properties->setProperties(ISATTACKING, cocos2d::Value(true));
	return;
}

void Monster::defend(void) {
	if (_properties->getValue(ISONDEFENSE).asBool()) {
		return;
	}
	auto animation = Anim::createAnimation("res\\monster\\left hand\\", "fire", "_defend", 1, 23, 0.05);
	auto callfunc = cocos2d::CallFunc::create([&]() {
		_properties->setProperties(ISONDEFENSE, cocos2d::Value(false));
	});
	_sprite->getLeftHandSprite()->stopAllActions();
	_sprite->getLeftHandSprite()->runAction(
		cocos2d::Sequence::create(cocos2d::Animate::create(animation), callfunc, nullptr));

	_properties->setProperties(ISONDEFENSE, cocos2d::Value(true));
	return;
}

void Monster::reset(void) {
	_properties->setProperties(ALIVE, cocos2d::Value(true));
	_properties->setProperties(HP, _properties->getValue(MAXHP));
	_properties->setProperties(STAMINA, _properties->getValue(MAXSTAMINA));
	setInitialCoordinate(
		_properties->getValue(INITPOSITIONX).asDouble(),
		_properties->getValue(INITPOSITIONY).asDouble());
	setVisible(true);
	setOrientation(_properties->getValue(INITORIENT).asDouble());
	auto animation = Anim::createAnimation("res\\monster\\monster body animation\\",
		"rat", "_animation", 1, 9, 0.1);
	animation->setLoops(-1);
	_sprite->getBodySprite()->runAction(cocos2d::Animate::create(animation));
}