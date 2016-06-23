/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by SXKDZ, DM
 */

#include "Entity.h"
#include "SceneOne.h"
#include "EventHandle.h"
#include <map>
#include <cmath>
#include <string>

std::map<EventName, EventBase *> EventHandle::_eventNameMap;
std::vector<EventBase *> EventHandle::_eventList;
Player *EventBase::player;

bool EventMonsterMark::verify(Entity *e) {
	if (e->getEntityType() != MONSTER) {
		return false;
	}
	Monster *monster = static_cast<Monster *>(e);
	auto playerPos = player->getPosition();
	auto monsterPos = monster->getPosition();
	
	double dis = playerPos.getDistance(monsterPos);
	double radius = monster->getProperties()->getValue(TERRITORYRADIUS).asDouble();
	if (dis <= radius) {
		return true;
	}
	monster->undoTargetting();
	return false;
}

void EventMonsterMark::execute(Entity *e) {
	Monster *monster = static_cast<Monster *>(e);
	if (!monster->getProperties()->getValue(ISTARGETTING).asBool() && !monster->getProperties()->getValue(ISVERIFING).asBool()) {
		monster->target(player);
	}
	return;
}

bool EventPassbyNPC::verify(Entity *e) {
	if (e->getEntityType() != NEUTRAL) {
		return false;
	}
	NPC *neutral = static_cast<NPC *> (e);
	auto playerPos = player->getPosition();
	auto neutralPos = neutral->getPosition();
	double dis = playerPos.getDistance(neutralPos);
	double radius = neutral->getProperties()->getValue(TERRITORYRADIUS).asDouble();
	if (dis <= radius) {
		return true;
	}
	neutral->hideDialogue();
	return false;
}

void EventPassbyNPC::execute(Entity *e) {
	NPC *neutral = static_cast<NPC *> (e);
	if (!neutral->getIsTalking()) {
		neutral->showDialogue();
	}
	return;
}

bool EventCollision::verify(Entity *e) {
	bool flag = false;
	Entity *majorEntity;
	Entity *minorEntity;

	for (int i = 0; i != 2; ++i) {
		if (i == 0) {
			majorEntity = player;
			minorEntity = e;
		}
		else {
			majorEntity = e;
			minorEntity = player;
		}
		std::vector<double> angleVec = majorEntity->getBody()->angle;
		std::vector<double> radiusVec = majorEntity->getBody()->radius;

		double orientation = majorEntity->getProperties()->getValue(ORIENTATION).asFloat() / 180 * M_PI;

		auto angleVecBeg = angleVec.cbegin();
		auto angleVecEnd = angleVec.cend();
		auto radiusVecBeg = radiusVec.cbegin();
		auto radiusVecEnd = radiusVec.cend();

		if (majorEntity->getProperties()->getValue(ISMOVINGFORWARD).asBool()) {
			angleVecEnd = angleVec.cbegin() + 3;
			radiusVecEnd = radiusVec.cbegin() + 3;
		}
		if (majorEntity->getProperties()->getValue(ISMOVINGBACKWARD).asBool()) {
			angleVecBeg = angleVec.cend() - 3;
			radiusVecBeg = radiusVec.cend() - 3;
		}
		for (; angleVecBeg != angleVecEnd; ++angleVecBeg, ++radiusVecBeg) {
			cocos2d::Point pCur = majorEntity->getPosition() + cocos2d::Vec2(*radiusVecBeg * cos(orientation + *angleVecBeg),
				*radiusVecBeg * (-1) * sin(orientation + *angleVecBeg));

			double distance =pCur.getDistance(minorEntity->getPosition());

			if (distance <= minorEntity->getBody()->broundaryRadiu) {
				flag = true;
			}
		}
	}

	majorEntity->getProperties()->setProperties(ISVERIFING, cocos2d::Value(flag));
	minorEntity->getProperties()->setProperties(ISVERIFING, cocos2d::Value(flag));
	return flag;
}

void EventCollision::execute(Entity *e) {
	SceneMapBase * parentScene = dynamic_cast<SceneMapBase *>(player->getScene());
	if (player->getProperties()->getValue(ISMOVINGFORWARD).asBool()) {
		// move twice: due to the different speeds of moving forward and backward
		player->moveBackward();
		player->moveBackward();
	}
	else if (player->getProperties()->getValue(ISMOVINGBACKWARD).asBool()) {
		player->moveForward();
	}
	return;
}

bool EventAttack::verify(Entity *e) {
	if (!player->getProperties()->getValue(STARTATTACK).asBool()) {
		return false;
	}
	// if player starts to attack, then set this property false
	if (e->getProperties()->getValue(ISHIT).asBool()) {
		return false;
	}
	auto attackArea = player->getAttackArea();
	auto pPos = player->getPosition();
	auto ePos = e->getPosition();
	auto pOrientation = player->getProperties()->getValue(ORIENTATION).asDouble();
	auto eOrientation = INSCRIBED_ANGLE 
		- atan2((ePos.y - pPos.y), (ePos.x - pPos.x)) / M_PI * INSCRIBED_ANGLE / 2; // clockwise
	while (eOrientation > INSCRIBED_ANGLE) {
		eOrientation -= INSCRIBED_ANGLE; // In case that the angle is over 360
	}
	auto areaSum = attackArea->angle.size();
	for (unsigned int i = 0; i < areaSum; ++i) {
		if (eOrientation < pOrientation - attackArea->angle[i] / 2
			|| eOrientation > pOrientation + attackArea->angle[i] / 2) { 
			// divides the attack angle into 2 parts
			continue;
		}
		if (ePos.distance(pPos + attackArea->vec[i]) <= attackArea->radius[i]) {
			// Only when the central point of the entity is in the area will it be attacked
			return true;
		}
	}
	return false;
}

void EventAttack::execute(Entity *e) {
	// It has been written in the function of update 
	if (e->getEntityType() == MONSTER) {
		Monster *monster = static_cast<Monster *>(e);
		if (monster->hit(player->getProperties()->getValue(ATK).asDouble())) {
			auto exp = player->getProperties()->getValue(EXPERIENCE).asDouble()
					+ monster->getProperties()->getValue(EXPERIENCE).asDouble();
			player->getProperties()->setProperties(EXPERIENCE, cocos2d::Value(exp));
		}
	}
	else if (e->getEntityType() == SUBSTANCE) {
		Substance *substance = static_cast<Substance *>(e);
		if (substance->hit()) {
			if (substance->getSubstanceType() == REVIVALSTONE) {
				auto pos = substance->getPosition();
				player->getProperties()->setProperties(INITPOSITIONX, cocos2d::Value(pos.x));
				player->getProperties()->setProperties(INITPOSITIONY, cocos2d::Value(pos.y));
				substance->getProperties()->setProperties(ALIVE, cocos2d::Value(false));
				substance->getSprite()->setColor(cocos2d::Color3B(0, 154, 73));
				return;
			}
			if (substance->getSubstanceType() == TREASURECHEST) {
				auto content = substance->getContent();
				if (content->getEquipType() == WEAPON) {
					cocos2d::Sprite *weapon = cocos2d::Sprite::create("res\\player\\weapon.png");
					cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();
					weapon->setPosition(cocos2d::Vec2(size.width / 10 - 100, size.height / 5));

					SceneMapBase *scene = dynamic_cast<SceneMapBase *>(player->getScene());
					GameItemLayer *itemLayer = dynamic_cast<GameItemLayer *>(scene->getChildByTag(10));

					cocos2d::log("weapon!");
					itemLayer->addChild(weapon, 5);
					player->addEquip(content);
				}
				else {
					player->useEquipOnce(content);
					player->checkHP();
				}
			}
			else {
				substance->destroy();
				substance->getProperties()->setProperties(ALIVE, cocos2d::Value(false));
			}
			// if the thing is broken, then get the tools inside and use it 
		}
		if (substance->getSubstanceType() == TREASURECHEST && !substance->getFirstOpen()) {
			substance->changeState();
		}
	}
	return;
}

bool EventMonsterAutomove::verify(Entity *e) {
	if (e->getEntityType() != MONSTER) {
		return false;
	}
	return (e->getProperties()->getValue(ISIDLE).asBool());
}

void EventMonsterAutomove::execute(Entity *e) {
	auto monster = static_cast<Monster *>(e);
	//monster->automove();
}

bool EventMonsterChase::verify(Entity *e) {
	if (e->getEntityType() != MONSTER) {
		return false;
	}
	return (e->getProperties()->getValue(ISTARGETTING).asBool());
}

void EventMonsterChase::execute(Entity *e) {
	auto monster = static_cast<Monster *>(e);
	monster->chase();
}

bool EventMonsterAttack::verify(Entity *e) {
	if (e->getEntityType() != MONSTER || e->getProperties()->getValue(ISATTACKING).asBool()
			|| !e->staminaEnough()) {
		return false;
	}
	Monster *monster = static_cast<Monster *>(e);
	auto attackArea = monster->getAttackArea();
	auto pPos = player->getPosition();
	auto mPos = monster->getPosition();
	auto mOrientation = monster->getProperties()->getValue(ORIENTATION).asDouble();
	auto pOrientation = INSCRIBED_ANGLE - atan2((pPos.y - mPos.y), (pPos.x - mPos.x)) / M_PI * INSCRIBED_ANGLE / 2; // clockwise
	while (pOrientation > INSCRIBED_ANGLE) {
		pOrientation -= INSCRIBED_ANGLE; // In case that the angle is over 360
	}
	auto areaSum = attackArea->angle.size();
	for (unsigned int i = 0; i < areaSum; ++i) {
		if (pOrientation < mOrientation - attackArea->angle[i] / 2
			|| pOrientation > mOrientation + attackArea->angle[i] / 2) {
			// divides the attack angle into 2 parts
			continue;
		}
		if (pPos.distance(mPos + attackArea->vec[i]) <= attackArea->radius[i]) {
			// Only when the central point of the entity is in the area will it be attacked
			return true;
		}
	}
	return false;
}

void EventMonsterAttack::execute(Entity *e) {
	auto monster = static_cast<Monster *>(e);
	if (monster->staminaEnough()) {
		monster->attack();
		player->hit(monster->getProperties()->getValue(ATK).asDouble());
	}
	return;
}

EventHandle::EventHandle() { }

void EventHandle::registerEventHandle(Player * p) {
	//associate the enumName with the Event*
	EventBase::bind(p); //bind once.

	EventBase* e = new EventMonsterMark;
	_eventList.push_back(e);
	_eventNameMap[EVENT_MONSTER_MARK] = e;

	e = new EventPassbyNPC;
	_eventList.push_back(e);
	_eventNameMap[EVENT_PASSBY_NPC] = e;

	e = new EventCollision;
	_eventList.push_back(e);
	_eventNameMap[EVENT_COLLISION] = e;

	e = new EventAttack;
	_eventList.push_back(e);
	_eventNameMap[EVENT_ATTACK] = e;

	e = new EventMonsterAutomove;
	_eventList.push_back(e);
	_eventNameMap[EVENT_MONSTER_AUTOMOVE] = e;

	e = new EventMonsterChase;
	_eventList.push_back(e);
	_eventNameMap[EVENT_MONSTER_CHASE] = e;

	e = new EventMonsterAttack;
	_eventList.push_back(e);
	_eventNameMap[EVENT_MONSTER_ATTACK] = e;
}

EventHandle::~EventHandle() {
	/*
	 * Maybe there will be problems of memory management, cause I delete the content of a static member
	 * Or should I keep it before all the scene finished?
	 * Any advice£¿
	 */
	for (auto event : _eventList) {
		delete event;
	}
	_eventList.clear();
}

void EventHandle::eventSwitch(EventName name, bool f) {
	auto event = _eventNameMap[name];
	if (f) {
		event->turnOn(); 
	} else {
		event->turnOff();
	}
	return;
}

void EventHandle::runEvent(Entity *entity) {
	for (auto it : _eventList) {
		if (!it->isDisabled() && it->verify(entity)) {
			it->execute(entity);
		}
	}
	return;
}
