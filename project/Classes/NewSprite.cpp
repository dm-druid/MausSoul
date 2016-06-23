/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by nemoremold
 */

#include "NewSprite.h"

constexpr double POSITION_ADJUSTMENTX = 3.0 / 16.0;
constexpr double POSITION_ADJUSTMENTY = 0;

NewSprite::NewSprite(void) {
	_body = nullptr;
	_leftHand = nullptr;
	_rightHand = nullptr;

	_name = "NULL";
	return;
}

bool NewSprite::init(void) {
	return true;
}

void NewSprite::bindBodySprite(cocos2d::Sprite *sprite) {
	if (!sprite) {
		throw std::string("bind bodySprite Error\n");
		return;
	}
	_body = sprite;
	_body->setPosition(cocos2d::Vec2::ZERO);
	this->addChild(_body);

	cocos2d::Size size = sprite->getContentSize();
	setContentSize(size);
	return;
}

void NewSprite::bindLeftHandSprite(const std::string &path) {
	auto lft = cocos2d::Sprite::create(path);
	_leftHand = lft;
	auto size = lft->getContentSize();
	_leftHand->setPosition(
		cocos2d::Point(size.width * POSITION_ADJUSTMENTX * 5 / 2,
			size.height * POSITION_ADJUSTMENTX * 3 / 2));
	_body->addChild(_leftHand);
	return;
}

void NewSprite::bindRightHandSprite(const std::string &path) {
	auto rgt = cocos2d::Sprite::create(path);
	_rightHand = rgt;
	auto size = rgt->getContentSize();
	_rightHand->setPosition(cocos2d::Point(size.width * POSITION_ADJUSTMENTX, POSITION_ADJUSTMENTY));
	addChild(_rightHand);
	return;
}

void NewSprite::setSpriteName(const std::string &name) {
	_name = name;
	return;
}

cocos2d::Sprite *NewSprite::getBodySprite(void) {
	return _body;
}

cocos2d::Sprite *NewSprite::getLeftHandSprite(void) {
	return _leftHand;
}

cocos2d::Sprite *NewSprite::getRightHandSprite(void) {
	return _rightHand;
}