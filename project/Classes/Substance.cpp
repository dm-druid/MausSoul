/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#include "Substance.h"

Substance::~Substance(void) {
	delete _content; // equip class does not have the construct funtion
}

bool Substance::init(void) {
	_state = 1;
	_firstOpen = true;
	//_breakable = false;
	_timesToBeBroken = TIMES_TO_BE_BROKEN;
	setEntityType(SUBSTANCE);
	return true;
}

void Substance::bindSprite2nd(cocos2d::Sprite *s) {
	addChild(s);
	
	_sprite2nd = s;
	s->setVisible(false); // add the second sprite and hide it
	
	auto point = cocos2d::Point(_properties->getValue(INITPOSITIONX).asDouble(),
		_properties->getValue(INITPOSITIONY).asDouble());
	s->setPosition(point);
	return;
}

void Substance::changeState(void) {
	if (_substanceType != TREASURECHEST) {
		return;
	}
	if (_state == 1) {
		auto animation = Anim::createAnimation("res\\objects\\objects interacted\\",
			"chest", "_open", 1, 5, 0.1);
		auto callfunc = cocos2d::CallFunc::create([&]() {
			_sprite->setVisible(false);
			_sprite2nd->setVisible(true);
		});
		_sprite->getBodySprite()->runAction(cocos2d::Sequence::create(
			cocos2d::Animate::create(animation), callfunc, nullptr));
	}
	else {
		auto animation = Anim::createAnimation(
			"res\\objects\\objects interacted\\",
			"chest", "_close", 1, 5, 0.1);
		auto callfunc = cocos2d::CallFunc::create([&]() {
			_sprite->setVisible(true);
			_sprite2nd->setVisible(false);
		});
		_sprite2nd->runAction(cocos2d::Sequence::create(
			cocos2d::Animate::create(animation), callfunc, nullptr));
	}
	_state = 1 - _state;
	return;
}

void Substance::setSubstanceType(SubstanceType st) {
	_substanceType = st;
	return;
}

SubstanceType Substance::getSubstanceType(void) {
	return _substanceType;
}

void Substance::bindOneTimeEquip(Equip *e) {
	_content = e;
	return;
}

Equip *Substance::getContent(void) {
	return _content;
}

bool Substance::hit(void) {
	--_timesToBeBroken;
	_properties->setProperties(ISHIT, cocos2d::Value(true));
	cocos2d::log("%d", _timesToBeBroken);
	scheduleOnce(schedule_selector(Substance::unhit), 1);
	if (_timesToBeBroken <= 0) {
		if (_firstOpen) { 
			_firstOpen = false;
			return true;
		}
	}
	return false;
}

void Substance::unhit(float delay) {
	_properties->setProperties(ISHIT, cocos2d::Value(false));
	return;
}

void Substance::destroy(void) {
	auto callfunc = cocos2d::CallFunc::create([&]() {
		_sprite->setVisible(false); 
	});
	cocos2d::Animation *animation;
	switch (_substanceType) {
	case BARREL1:	
		animation = Anim::createAnimation("res\\objects\\objects destroyed\\",
			"barrel1", "_destroyed", 1, 15, 0.1);
		break;
	case BARREL2:
		animation = Anim::createAnimation("res\\objects\\objects destroyed\\",
			"barrel2", "_destroyed", 1, 15, 0.1);
		break;
	case BOX1:
		animation = Anim::createAnimation("res\\objects\\objects destroyed\\",
			"box1", "_destroyed", 1, 16, 0.1);
		break;
	case BOX2:
		animation = Anim::createAnimation("res\\objects\\objects destroyed\\",
			"box2", "_destroyed", 1, 15, 0.1);
		break;
	case FIRECHAIR:
		animation = Anim::createAnimation("res\\objects\\objects destroyed\\",
			"chair", "_destroyed", 1, 15, 0.1);
		break;
	default:
		return;
		break;
	}
	_sprite->getBodySprite()->runAction(
		cocos2d::Sequence::create(cocos2d::Animate::create(animation), callfunc, nullptr));
}

bool Substance::getFirstOpen(void) {
	return _firstOpen;
}
