/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by nemoremold, DM
 */

#include "Entity.h"

Entity::Entity(void) {
	_sprite = NULL;
	_equipMap[CLOTH] = NULL;
	_equipMap[SHOES] = NULL;
	_equipMap[WEAPON] = NULL;
	_equipMap[SOMETHING] = NULL;

	Equip *defaultEquip = new Equip;
	defaultEquip->setAddValue(ADDATK, cocos2d::Value(0));
	defaultEquip->setEquipType(WEAPON);
	defaultEquip->setAttackArea({ { cocos2d::Vec2::ZERO },{ 200 },{ 60 } });
	defaultEquip->setName("defaultEquip");
	selectedEquip = defaultEquip;
	_equips.push_back(defaultEquip);
	_coordinate = cocos2d::Point(0, 0);
	_target = NULL;
	_properties = new EntityProperties;
	return;
}

void Entity::reset(void) {
	_properties->setProperties(ALIVE, cocos2d::Value(true));
	_properties->setProperties(HP, _properties->getValue(MAXHP));
	_properties->setProperties(ISHIT, cocos2d::Value(false));
	_properties->setProperties(ISATTACKING, cocos2d::Value(false));
	setInitialCoordinate(_properties->getValue(INITPOSITIONX).asDouble(), _properties->getValue(INITPOSITIONY).asDouble());
	setVisible(true);
	return;
}

void Entity::setAttackArea(AttackArea a) {
	_attackArea = a;
	return;
}

AttackArea *Entity::getAttackArea() {
	return &_attackArea;
}

void Entity::setBody(Body b) {
	_body = b;
	return;
}

Body *Entity::getBody(void) {
	return &_body;
}

void Entity::setEntityName(const std::string &s) {
	_name = s;
	return;
}

std::string &Entity::getEntityName() {
	return _name;
}

void Entity::setEntityType(EntityType t) {
	_entityType = t;
	return;
}

EntityType Entity::getEntityType() {
	return _entityType;
}

Entity::~Entity(void) {
	_target = NULL;
	delete _properties;
	return;
}

NewSprite *Entity::getSprite(void) {
	return this->_sprite;
}

void Entity::bindSprite(NewSprite *spr) {
	if (spr == NULL) {
		return;
	}
	_sprite = spr;
	addChild(_sprite);
	
	//set Entity's contentsize
	cocos2d::Size size = spr->getContentSize();
	setContentSize(size);
	return;
}

cocos2d::Point Entity::getPosition(void) {
	return _coordinate;
}

double Entity::getOrientation() {
	return _properties->getValue(ORIENTATION).asDouble();
}
/*
 * Property System:
 * wear() takeoff()s and useEquipOnce are all connected
 * with the equipment system
 * deals with conditions of changing or use of equipments
 */

EntityProperties *Entity::getProperties(void) {
	return _properties;
}

cocos2d::Point Entity::calcForwardPoint(void) {
	cocos2d::Point pos;
	pos.x = _coordinate.x
		+ _properties->getValue(VELOCITY).asDouble()
		* cos(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180);
	pos.y = _coordinate.y
		- _properties->getValue(VELOCITY).asDouble()
		* sin(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180);
	return pos;
}

cocos2d::Point Entity::calcBackwardPoint(void) { 
	cocos2d::Point pos;
	pos.x = _coordinate.x 
		- _properties->getValue(VELOCITY).asDouble()
		* cos(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180) / 2;
	pos.y = _coordinate.y
		+ _properties->getValue(VELOCITY).asDouble() 
		* sin(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180) / 2;
	return pos;
}

bool Entity::wear(Equip *equip) {
	if (!equip) {
		return false;
	}
	if (!_equipMap[equip->getEquipType()]) {
		takeoff(_equipMap[equip->getEquipType()]);
	}
	_equipMap[equip->getEquipType()] = equip;
	
	_properties->addProperties(HP, equip->getAddValue(addValue::ADDHP));
	_properties->addProperties(ATK, equip->getAddValue(addValue::ADDATK));
	_properties->addProperties(VELOCITY, equip->getAddValue(addValue::ADDSPEED));
	_properties->addProperties(DEFENCE, equip->getAddValue(addValue::ADDDEFENCE));

	_attackArea = equip->getAttackArea();
	return true;
}

bool Entity::takeoff(Equip *equip) {
	if (!equip) {
		return false;
	}
	_equipMap[equip->getEquipType()] = nullptr;
	_properties->minusProperties(HP, equip->getAddValue(addValue::ADDHP));
	_properties->minusProperties(ATK, equip->getAddValue(addValue::ADDATK));
	_properties->minusProperties(VELOCITY, equip->getAddValue(addValue::ADDSPEED));
	_properties->minusProperties(DEFENCE, equip->getAddValue(addValue::ADDDEFENCE));
	return true;
}

bool Entity::takeoff(equipType eT) {
	Equip *equip = _equipMap[eT];
	if (!equip) {
		return false;
	}
	_equipMap[equip->getEquipType()] = nullptr;
	_properties->minusProperties(HP, equip->getAddValue(addValue::ADDHP));
	_properties->minusProperties(ATK, equip->getAddValue(addValue::ADDATK));
	_properties->minusProperties(VELOCITY, equip->getAddValue(addValue::ADDSPEED));
	_properties->minusProperties(DEFENCE, equip->getAddValue(addValue::ADDDEFENCE));
	return true;
}

bool Entity::useEquipOnce(Equip *equip) {
	if (!equip && equip->canEffectOnce()) {
		return false;
	}
	_properties->addProperties(HP, equip->getAddOnce(addOnce::ADDONCEHP));
	_properties->addProperties(ATK, equip->getAddOnce(addOnce::ADDONCEATK));
	_properties->addProperties(VELOCITY, equip->getAddOnce(addOnce::ADDONCESPEED));
	_properties->addProperties(DEFENCE, equip->getAddOnce(addOnce::ADDONCEDEFENCE));
	equip->useOnce();
	return true;
}

/*
 * Action System:
 * status settings are for initialization or later needed operations
 * where as action system functions are methods for event handle to use
 * after the merge, this system is now compitable with the property system
 */

// status initializatized settings
void Entity::setInitialCoordinate(double x, double y) {
	// x, y refer to the x-coordinate and y-coordinate
	_properties->setProperties(INITPOSITIONX, cocos2d::Value(x));
	_properties->setProperties(INITPOSITIONY, cocos2d::Value(y));
	_coordinate.x = x;
	_coordinate.y = y;
	_sprite->setPosition(_coordinate.x, _coordinate.y);
	return;
}

void Entity::setSpeed(double v) {
	// v refers to speed. method used for changing the default velocity
	_properties->minusProperties(VELOCITY, _properties->getValue(DEFAULTVELOCITY));
	// current velocity is influenced by default velocity
	_properties->setProperties(DEFAULTVELOCITY, cocos2d::Value(v));
	_properties->addProperties(VELOCITY, cocos2d::Value(v));
	return;
}

void Entity::setAcceleration(double a) {
	// a refers to acceleration speed
	_properties->setProperties(ACCELERATION, cocos2d::Value(a));
}

void Entity::setAngularSpeed(double w) {
	// w refers to angular speed
	_properties->setProperties(ANGULARVELOCITY, cocos2d::Value(w));
	return;
}

void Entity::setOrientation(double o) {
	// o refers to the facing orientation of the entity
	_properties->setProperties(ORIENTATION, cocos2d::Value(o));
	_sprite->setRotation(o);
	return;
}

// actionSystem methods
/*
 * NOTICE:
 * formulas below don't need understanding;
 * formulas control the movement of the sprite;
 * cos() and sin() use arc numbers as parameters;
 *
 * FOR NOW THE SYSTEM IS NOT COMPLETE, LACKING ANIMATIONS AND DETAILED CALCULATIONS
 * PLEASE CHANGE NOTHING BEFORE INFORMING nemoremold
 */
void Entity::moveForward(void) {
	_properties->setProperties(ISMOVING, cocos2d::Value(true));
	_properties->setProperties(ISIDLE, cocos2d::Value(false));
	_properties->setProperties(ISMOVINGFORWARD, cocos2d::Value(true));
	_properties->setProperties(ISMOVINGBACKWARD, cocos2d::Value(false));
	_coordinate = calcForwardPoint();
	_sprite->setPosition(_coordinate.x, _coordinate.y);
	return;
}

void Entity::moveBackward(void) {
	_properties->setProperties(ISMOVING, cocos2d::Value(true));
	_properties->setProperties(ISIDLE, cocos2d::Value(false));
	_properties->setProperties(ISMOVINGFORWARD, cocos2d::Value(false));
	_properties->setProperties(ISMOVINGBACKWARD, cocos2d::Value(true));
	_coordinate = calcBackwardPoint();
	_sprite->setPosition(_coordinate.x, _coordinate.y);
	return;
}

void Entity::sprint(void) {
	// an accelerating system for further use
	lossStamina();
	if (_properties->getValue(VELOCITY).asDouble()
		+ _properties->getValue(ACCELERATION).asDouble() <= MAX_VELOCITY) {
		_properties->addProperties(VELOCITY, _properties->getValue(ACCELERATION));
	}
	else {
		_properties->setProperties(VELOCITY, cocos2d::Value(MAX_VELOCITY));
		cocos2d::log("max speed\n");
	}
	return;
}

void Entity::rotateRight(void) {
	_properties->setProperties(ISROTATING, cocos2d::Value(true));
	_properties->setProperties(ROTATINGRIGHT, cocos2d::Value(true));
	_properties->setProperties(ISIDLE, cocos2d::Value(false));
	if (!_properties->getValue(ISTARGETTING).asBool()) {
		// if not targetting right now, entity rotates
		_properties->addProperties(ORIENTATION, _properties->getValue(ANGULARVELOCITY));
		if (_properties->getValue(ORIENTATION).asDouble() >= 360) {
			_properties->minusProperties(ORIENTATION, cocos2d::Value(360));
		}
		_sprite->setRotation(_properties->getValue(ORIENTATION).asDouble());
	}
	else {
		// if targetting right now, entity moves laterally with half the speed
		_coordinate.x -= (_properties->getValue(VELOCITY).asDouble()
			* sin(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180) / 2);
		_coordinate.y -= (_properties->getValue(VELOCITY).asDouble() 
			* cos(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180) / 2);
		_sprite->setPosition(_coordinate.x, _coordinate.y);
	}
	return;
}

void Entity::rotateLeft(void) {
	_properties->setProperties(ISROTATING, cocos2d::Value(true));
	_properties->setProperties(ROTATINGLEFT, cocos2d::Value(true));
	_properties->setProperties(ISIDLE, cocos2d::Value(false));
	if (!_properties->getValue(ISTARGETTING).asBool()) {
		// if not targetting right now, entity rotates
		_properties->minusProperties(ORIENTATION, _properties->getValue(ANGULARVELOCITY));
		if (_properties->getValue(ORIENTATION).asDouble() <= 0) {
			_properties->addProperties(ORIENTATION, cocos2d::Value(360));
		}
		_sprite->setRotation(_properties->getValue(ORIENTATION).asDouble());
	}
	else {
		// if targetting right now, entity moves laterally with half the speed
		_coordinate.x += (_properties->getValue(VELOCITY).asDouble() 
			* sin(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180) / 2);
		_coordinate.y += (_properties->getValue(VELOCITY).asDouble()
			* cos(_properties->getValue(ORIENTATION).asDouble() * M_PI / 180) / 2);
		_sprite->setPosition(_coordinate.x, _coordinate.y);
	}
	return;
}

void Entity::target(Entity *target) {
	if (!_properties->getValue(ISTARGETTING).asBool()) {
		_properties->setProperties(ISTARGETTING, cocos2d::Value(true));
	}
	_properties->setProperties(ISIDLE, cocos2d::Value(false));
	_target = target;
	return;
}

void Entity::undoTargetting(void) {
	if (_properties->getValue(ISTARGETTING).asBool()) {
		_properties->setProperties(ISTARGETTING, cocos2d::Value(false));
		_target = NULL;
		idle();
	}
	return;
}

void Entity::stopMovement(void) {
	_sprite->getBodySprite()->stopAllActions();
	_properties->setProperties(ISIDLE, cocos2d::Value(true));
	_properties->setProperties(ISMOVING, cocos2d::Value(false));
	_properties->setProperties(ISROTATING, cocos2d::Value(false));
	_properties->setProperties(ISMOVABLE, cocos2d::Value(false));
	_properties->setProperties(ISATTACKING, cocos2d::Value(false));
	_properties->setProperties(ISONDEFENSE, cocos2d::Value(false));
	return;
}

void Entity::spawn(const cocos2d::Point &location) {
	stopMovement();
	return;
}

bool Entity::staminaEnough(void) {
	if (_properties->getValue(STAMINA).asDouble() < _properties->getValue(STAMINALOSS).asDouble()) {
		return false;
	}
	return true;
}

void Entity::minusStamina(void) {
	if (staminaEnough()) {
		_properties->minusProperties(STAMINA, _properties->getValue(STAMINALOSS));
	}
	return;
}

void Entity::recoverStamina(void) {
	auto sta = _properties->getValue(STAMINA).asDouble() + _properties->getValue(RECOVERSTAMINA).asDouble();
	if (sta < _properties->getValue(MAXSTAMINA).asDouble() && sta >= 0.0) {
		_properties->setProperties(STAMINA, cocos2d::Value(sta));
	}
	return;
}

void Entity::lossStamina(void) {
	auto sta = _properties->getValue(STAMINA).asDouble() - 1.5 * _properties->getValue(RECOVERSTAMINA).asDouble();
	if (sta < _properties->getValue(MAXSTAMINA).asDouble() && sta >= 0.0) {
		_properties->setProperties(STAMINA, cocos2d::Value(sta));
	}
	return;
}

void Entity::idle(void) {
	_properties->setProperties(ISIDLE, cocos2d::Value(true));
	return;
}

void Entity::moderate(void) {
	// part of accelerating system;used for moderations
	if (_properties->getValue(VELOCITY).asDouble() -
		2 * _properties->getValue(ACCELERATION).asDouble()
			> _properties->getValue(DEFAULTVELOCITY).asDouble()) {
		_properties->minusProperties(VELOCITY,
			cocos2d::Value(2 * _properties->getValue(ACCELERATION).asDouble()));
	}
	else {
		_properties->setProperties(VELOCITY, _properties->getValue(DEFAULTVELOCITY));
	}
	return;
}

void Entity::moveByDis(double dis) {
	_properties->setProperties(ISMOVING, cocos2d::Value(true));
	_properties->setProperties(ISIDLE, cocos2d::Value(false));

	double time = dis / _properties->getValue(VELOCITY).asDouble();
	return;
}

void Entity::rotateByDegree(double degree) {
	// when degree is positive, entity rotates rightwards
	if (degree < 0) {
		degree += ((int)(degree / 360) + 1) * 360;
	}
	else if (degree > 0) {
		degree -= ((int)(degree / 360)) * 360;
	}
	else {
		return;
	}

	_properties->addProperties(ORIENTATION, cocos2d::Value(degree));
	if (_properties->getValue(ORIENTATION).asDouble() < 0) {
		_properties->addProperties(ORIENTATION, cocos2d::Value(360));
	}
	else if (_properties->getValue(ORIENTATION).asDouble() >= 360) {
		_properties->minusProperties(ORIENTATION, cocos2d::Value(360));
	}
	_sprite->setRotation(_properties->getValue(ORIENTATION).asDouble());
	return;
}

void Entity::chase(void) {
	if (_target == NULL) {
		return;
	}
	constexpr double INSCRIBED_ANGLE = 360;
	cocos2d::Point tPos = _target->getPosition();
	cocos2d::Point ePos = getPosition();
	auto tOrientation = INSCRIBED_ANGLE
		- atan2((tPos.y - ePos.y), (tPos.x - ePos.x)) / M_PI * INSCRIBED_ANGLE / 2; // clockwise
	while (tOrientation > INSCRIBED_ANGLE) {
		tOrientation -= INSCRIBED_ANGLE; // In case that the angle is over 360
	}
	setOrientation(tOrientation);
	if (!_properties->getValue(ISATTACKING).asBool() && !_properties->getValue(ISVERIFING).asBool()) {
		moveForward();
	}
	else if(_properties->getValue(ISVERIFING).asBool())	{
		moveBackward();
	}
	return;
}

void Entity::changeEquips() {
	int equipMaxnum = _equips.size();
	int curNum = equipNum;
	++equipNum;
	if (equipNum + 1 > equipMaxnum) {
		equipNum -= equipMaxnum;
	}
	this->takeoff(_equips[curNum]);
	this->wear(_equips[equipNum]);
	selectedEquip = _equips[equipNum];

	cocos2d::Scene *scene = dynamic_cast<cocos2d::Scene *>(getScene());
	GameItemLayer *itemLayer = dynamic_cast<GameItemLayer *>(scene->getChildByTag(10));

}

void Entity::addEquip(Equip * _equip) {
	_equipMap.insert(std::make_pair(_equip->getEquipType(), _equip));
	_equips.push_back(_equip);

}
