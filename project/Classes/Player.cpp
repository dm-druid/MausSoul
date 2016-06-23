/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#include "Player.h"

Player::Player(void) { 
	setEntityType(PLAYER);
	setAttackArea({ {cocos2d::Vec2::ZERO}, {200.0}, {60.0} });
	return;
}

bool Player::init(void){
	selectedEquip = _equips.at(0);
	return true;
}

void Player::reset(void) {
	removeChild(_sprite);
	auto playerSprite = NewSprite::create();
	playerSprite->setSpriteName("player");
	cocos2d::Sprite *sprite = cocos2d::Sprite::create("res\\player\\rat.png");
	playerSprite->bindBodySprite(sprite);
	playerSprite->bindLeftHandSprite("res\\player\\left hand\\firestartpos.png");
	playerSprite->bindRightHandSprite("res\\player\\right hand\\firestartpos.png");
	bindSprite(playerSprite);
	
	setOrientation(270);
	_properties->setProperties(ALIVE, cocos2d::Value(true));
	_properties->setProperties(HP, _properties->getValue(MAXHP));
	_properties->setProperties(STAMINA, _properties->getValue(MAXSTAMINA));
	setInitialCoordinate(
		_properties->getValue(INITPOSITIONX).asDouble(),
		_properties->getValue(INITPOSITIONY).asDouble());
	setVisible(true);

	auto animation = Anim::createAnimation("res\\player\\rat body animation\\",
		"rat", "_animation", 1, 9, 0.1);
	animation->setLoops(-1);
	_sprite->getBodySprite()->runAction(cocos2d::Animate::create(animation));
}

void Player::attack(void) {
	if (_properties->getValue(ISATTACKING).asBool()) {
		return;
	}
	minusStamina();
	cocos2d::Animation *animation;
	if (selectedEquip->getName() == "defaultEquip") {
		animation = Anim::createAnimation(
			"res\\player\\right hand\\", "fire", "_attack", 1, 16, 0.05);
	}
	else {
		animation = Anim::createAnimation(
			"res\\player\\right hand\\", "bfire", "_attack", 1, 16, 0.05);
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

void Player::defend(void) {
	if (_properties->getValue(ISONDEFENSE).asBool()) {
		return;
	}
	minusStamina();
	auto animation = Anim::createAnimation(
		"res\\player\\left hand\\", "fire", "_defend", 1, 23, 0.05);
	auto callfunc = cocos2d::CallFunc::create([&]() {
		_properties->setProperties(ISONDEFENSE, cocos2d::Value(false));
	});
	_sprite->getLeftHandSprite()->stopAllActions();
	_sprite->getLeftHandSprite()->runAction(
		cocos2d::Sequence::create(cocos2d::Animate::create(animation), callfunc, nullptr));

	_properties->setProperties(ISONDEFENSE, cocos2d::Value(true));

	return;
}

void Player::hit(double atk) {
	if (_properties->getValue(ISONDEFENSE).asBool()){
		return;
	}
	_properties->minusProperties(HP, cocos2d::Value(atk));
	if (_properties->getValue(HP).asDouble() <= 0.0) {
		die();
	}
	return;
}

void Player::die(void) {
	stopMovement();
	_properties->setProperties(ALIVE, cocos2d::Value(false));
	auto animation = Anim::createAnimation("res\\player\\death animation\\",
		"player", "_death", 1, 28, 0.1, false);
	_sprite->getBodySprite()->runAction(cocos2d::Animate::create(animation));
	return;
}

double Player::getPercentHP() {
	double percent = _properties->getValue(HP).asDouble()
		/ _properties->getValue(MAXHP).asDouble() * 100.0;
	if (percent > 100.0) {
		return 100.0;
	}
	else if (percent < 0.0) {
		return 0.0;
	}
	else {
		return percent;
	}
}

double Player::getPercentStamina() {
	double percent = _properties->getValue(STAMINA).asDouble()
		/ _properties->getValue(MAXSTAMINA).asDouble() * 100.0;
	if (percent > 100.0) {
		return 100.0;
	}
	else {
		return percent;
	}
}

void Player::checkHP(void) {
	auto maxHP = _properties->getValue(MAXHP).asDouble();
	if (_properties->getValue(HP).asDouble() > maxHP) {
		_properties->setProperties(HP, cocos2d::Value(maxHP));
	}
	return;
}