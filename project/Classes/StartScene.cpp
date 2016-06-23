/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by lisirrx
 */

#include "StartScene.h"

bool StartScene::init(void) {
	auto director = cocos2d::Director::getInstance();
	auto size = cocos2d::Director::getInstance()->getVisibleSize();

	cocos2d::Label *beginLabel = cocos2d::Label::createWithTTF(
		"Start Game",
		"fonts\\FancyText.ttf", 100, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);

	begin = cocos2d::MenuItemLabel::create(
		beginLabel,
		CC_CALLBACK_1(StartScene::addReasources, this));
	begin->setPosition(size.width / 2, size.height / 3 + 100);
	cocos2d::Menu* pMenu1 = cocos2d::Menu::create(begin, NULL);
	pMenu1->setPosition(cocos2d::Point(0, 0));
	this->addChild(pMenu1, 1);

	cocos2d::Label *endLabel = cocos2d::Label::createWithTTF(
		"Exit Game",
		"fonts\\FancyText.ttf", 100, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
	end = cocos2d::MenuItemLabel::create(
		endLabel,
		CC_CALLBACK_1(StartScene::exitGame, this));
	end->setPosition(size.width / 2, size.height / 3 - 100);
	cocos2d::Menu* pMenu2 = cocos2d::Menu::create(end, NULL);
	pMenu2->setPosition(cocos2d::Point(0, 0));
	this->addChild(pMenu2, 1);

	title = cocos2d::Label::createWithTTF(
		"Maus      Soul",
		"fonts\\FancyText.ttf", 250, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
	title->setPosition(size.width / 2, size.height * 7 / 10);
	addChild(title);

	auto logo = cocos2d::Sprite::create("logo.png");
	logo->setPosition(size.width / 2, size.height * 7 / 10);
	logo->setScale(0.35f);
	logo->setOpacity(200);
	addChild(logo);

	return true;
}

void StartScene::exitGame(cocos2d::Ref *ref) {
	cocos2d::Director::getInstance()->end();
	return;
}

void StartScene::loadingCallback(cocos2d::Texture2D * pSender){
	auto director = cocos2d::Director::getInstance();

	++m_loadedSp;
	cocos2d::log("loaded: %d", m_loadedSp);
	if (m_loadedSp == m_numSp) {
		SceneMapBase *scene = SceneMapBase::create();
		director->replaceScene(cocos2d::TransitionFade::create(2.0f, scene));
	}
	return;
}

void StartScene::addReasources(Ref *ref) {
	auto director = cocos2d::Director::getInstance();

	begin->setVisible(false);
	end->setVisible(false);
	end->setEnabled(false);

	auto size = cocos2d::Director::getInstance()->getVisibleSize();
	auto load = cocos2d::Label::createWithTTF(
		"Loading",
		"fonts\\FancyText.ttf", 150, cocos2d::Size::ZERO, cocos2d::TextHAlignment::RIGHT);
	load->setPosition(size.width / 2, size.height * 1 / 3);
	addChild(load);

	director->getTextureCache()->addImageAsync("map\\cover1.1.1.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.1.2.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.1.3.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.1.4.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.1.5.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.2.1.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.2.2.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.2.3.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.2.4.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.2.5.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.3.1.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.3.2.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.3.3.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.3.4.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.3.5.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.4.1.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.4.2.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.4.3.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.4.4.png", CC_CALLBACK_1(StartScene::loadingCallback, this));
	director->getTextureCache()->addImageAsync("map\\cover1.4.5.png", CC_CALLBACK_1(StartScene::loadingCallback, this));

	return;
}
