#include "NPC.h"

constexpr double INIT_TERRITORY_RADIUS = 200;
constexpr double CONST_HEIGHT = 100;

NPC::NPC() {
	setEntityType(NEUTRAL);
	return;
}

bool NPC::init(void) {
	_properties->setProperties(TERRITORYRADIUS, cocos2d::Value(INIT_TERRITORY_RADIUS));
	return true;
}

void NPC::addDialogue(const std::string &path) {
	_dialogueSprite = cocos2d::Sprite::create(path);
	auto pos = getPosition();
	auto spriteSize = getSprite()->getContentSize();
	_dialogueSprite->setPosition(cocos2d::Point(pos.x, pos.y + spriteSize.height + CONST_HEIGHT));
	_dialogueSprite->setOpacity(200);
	_dialogueSprite->setScale(0, 0);
	//_dialogueSprite->setVisible(false);
	addChild(_dialogueSprite);
	_isTalking = false;
	return;
}

void NPC::showDialogue(void) {
	//_dialogueSprite->stopAllActions();
	_isTalking = true;
	auto scaleTo = cocos2d::ScaleTo::create(1.0, 1.0, 1.0);
	_dialogueSprite->runAction(scaleTo);
	return;
}

void NPC::hideDialogue(void) {
	_dialogueSprite->stopAllActions();
	_isTalking = false;
	_dialogueSprite->setScale(0.0, 0.0);
	/*auto callfunc = cocos2d::CallFunc::create([&]() 
	{	
		_dialogueSprite->setVisible(false);
	});
	auto scaleTo = cocos2d::ScaleTo::create(1.0, 1.0, 1.0);
	_dialogueSprite->runAction(cocos2d::Sequence::create(scaleTo, callfunc, nullptr));*/
	return;
}

