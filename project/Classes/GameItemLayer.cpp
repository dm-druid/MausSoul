/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by SXKDZ
 */

#include "GameItemLayer.h"
#include "cocos2d.h"

bool GameItemLayer::init(void) {
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	// TODO(DM or nemoremold): extract those image paths into property list
	_HPOuterHud = cocos2d::Sprite::create("ui\\HPOuterHud.png");
	_HPOuterHud->setAnchorPoint(cocos2d::Vec2(0, 1));
	_HPOuterHud->setPosition(cocos2d::Vec2(50, visibleSize.height - _HPOuterHud->getContentSize().height));
	addChild(_HPOuterHud);

	_HPInnerHud = cocos2d::Sprite::create("ui\\HPInnerHud.png");
	_HPInnerHud->setAnchorPoint(cocos2d::Vec2(0, 1));
	_HPInnerHud->setPosition(cocos2d::Vec2(55, visibleSize.height - _HPInnerHud->getContentSize().height - 7));
	addChild(_HPInnerHud);

	_StaminaOuterHud = cocos2d::Sprite::create("ui\\HPOuterHud.png");
	_StaminaOuterHud->setAnchorPoint(cocos2d::Vec2(0, 1));
	_StaminaOuterHud->setPosition(cocos2d::Vec2(50, visibleSize.height - 3 * _StaminaOuterHud->getContentSize().height));
	addChild(_StaminaOuterHud);

	_StaminaInnerHud = cocos2d::Sprite::create("ui\\HPInnerHud.png");
	_StaminaInnerHud->setColor(cocos2d::Color3B(16, 78, 139));
	_StaminaInnerHud->setAnchorPoint(cocos2d::Vec2(0, 1));
	_StaminaInnerHud->setPosition(cocos2d::Vec2(55, visibleSize.height - 2 * _StaminaOuterHud->getContentSize().height
			- _StaminaInnerHud->getContentSize().height - 7));
	addChild(_StaminaInnerHud);

	_ExpHudShadow = cocos2d::Sprite::create("ui\\ExpHud.png");
	_ExpHudShadow->setColor(cocos2d::Color3B(125, 125, 125));
	_ExpHudShadow->setAnchorPoint(cocos2d::Vec2(1, 0));
	_ExpHudShadow->setPosition(
		cocos2d::Vec2(visibleSize.width - _ExpHudShadow->getContentSize().width + 230, 80));
	addChild(_ExpHudShadow);

	_ExpHud = cocos2d::Label::createWithTTF(
		"0",
		"fonts\\Baskerville.ttc", 45, cocos2d::Size::ZERO, cocos2d::TextHAlignment::RIGHT);
	_ExpHud->setAnchorPoint(cocos2d::Vec2(1, 0));
	_ExpHud->setPosition(
		cocos2d::Vec2(visibleSize.width - _ExpHudShadow->getContentSize().width + 230, 80));
	_ExpHud->enableShadow();
	addChild(_ExpHud);

	const cocos2d::Vec2 posVec[4] = { {100, 0}, {-100, 0} };
	for (int i = 0; i < 2; ++i) {
		cocos2d::Sprite *weaponBox = cocos2d::Sprite::create("ui\\weaponBox.png");
		weaponBox->setOpacity(200);
		weaponBox->setPosition(cocos2d::Vec2(visibleSize.width / 10 + posVec[i].x, visibleSize.height / 5 + posVec[i].y - 60));
		addChild(weaponBox,2,i);
	}

	return true;
}

void GameItemLayer::setHP(double percentage) {
	if (percentage > 100.0 || percentage < 0.0) {
		throw std::string("percentage is out of range");
	}
	_HPInnerHud->setScaleX(percentage / 100.0f);

	return;
}

void GameItemLayer::setStamina(double percentage) {
	if (percentage > 100.0 || percentage < 0.0) {
		throw std::string("percentage is out of range");
	}
	_StaminaInnerHud->setScaleX(percentage / 100.0f);

	return;
}

void GameItemLayer::setExp(int exp) {
	cocos2d::Value v(exp);
	_ExpHud->setString(v.asString());
	return;
}
