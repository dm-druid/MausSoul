/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#include "DeadScene.h"
#include "SimpleAudioEngine.h"

bool DeadScene::init(void) {
	if (!Scene::init()) {
		return false;
	}

	label = cocos2d::Label::createWithTTF("Dead is dead?", "fonts\\Baskerville.ttc", 100);
	auto size = cocos2d::Director::getInstance()->getVisibleSize();
	label->setPosition(size.width / 2, size.height / 2);
	addChild(label);

	auto *pItem = cocos2d::MenuItemImage::create(
		"res\\button\\restart.png",
		"res\\button\\restart.png",
		CC_CALLBACK_1(DeadScene::popScene, this));
	pItem->setPosition(size.width / 2, size.height / 3);

	cocos2d::Menu* pMenu = cocos2d::Menu::create(pItem, NULL);
	pMenu->setPosition(cocos2d::Point(0, 0));
	addChild(pMenu, 1);

	audioSetup();

	return true;
}

void DeadScene::audioSetup(void) {
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	audio->preloadEffect("music\\death.mp3");
	audio->playEffect("music\\death.mp3");
	return;
}

cocos2d::Scene* DeadScene::createScene(void) {
	auto scene = cocos2d::Scene::create();
	auto layer = DeadScene::create();
	scene->addChild(layer);
	return scene;
}

void DeadScene::popScene(Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	cocos2d::Director::getInstance()->popScene();
	return;
}
