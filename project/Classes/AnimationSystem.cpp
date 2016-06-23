/*
* Project Pacific
* Copyright (c) 2016 Pacific Development Team. All rights reserved.
*
* written by nemoremold
*/

#include "AnimationSystem.h"

cocos2d::Animation* Anim::createAnimation(
	std::string root, std::string target, std::string animation,
	int start, int end, double delay) {
	auto *pRet = cocos2d::Animation::create();
	for (int i = start; i <= end; ++i) {
		// get the path of the file by combining strings together
		char buffer[20] = { 0 };
		sprintf(buffer, "_%d.png", i);
		std::string filePath = root + target + animation + buffer;
		pRet->addSpriteFrameWithFile(filePath);
	}
	pRet->setDelayPerUnit(delay);
	pRet->setRestoreOriginalFrame(true);
	return pRet;
}

cocos2d::Animation* Anim::createAnimation(
	std::string root, std::string target, std::string animation,
	int start, int end, double delay, bool type) {
	auto pRet = Anim::createAnimation(
		root, target, animation,
		start, end, delay);
	pRet->setRestoreOriginalFrame(type);
	return pRet;
}
