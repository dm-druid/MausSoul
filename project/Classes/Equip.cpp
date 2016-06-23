 /*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#include "Equip.h"

Equip::Equip(void) : _equipType(SOMETHING), _canEffectOnce(true) {
	_addOnceMap[ADDONCEHP] = 0;
	_addOnceMap[ADDONCEATK] = 0;
	_addOnceMap[ADDONCESPEED] = 0;
	_addOnceMap[ADDONCEDEFENCE] = 0;
	_addOnceMap[ADDONCEBEGIN] = 0;
	_addOnceMap[ADDONCEEND] = 0;

	_addMap[ADDHP] = 0;
	_addMap[ADDATK] = 0;
	_addMap[ADDSPEED] = 0;
	_addMap[ADDDEFENCE] = 0;
	_addMap[ADDVALUEBEGIN] = 0;
	_addMap[ADDVALUEEND] = 0;
	
	return;
}

void Equip::setEquipType(equipType etp) {
	_equipType = etp;
	return;
}

equipType Equip::getEquipType(void) {
	return _equipType;
}

void Equip::setAddOnce(addOnce item, cocos2d::Value val) {
	_addOnceMap[item] = val;
	return;
}

void Equip::setAddValue(addValue item, cocos2d::Value val) {
	_addMap[item] = val;
	return;
}

cocos2d::Value Equip::getAddValue(addValue item) {
	return _addMap[item];
}

cocos2d::Value Equip::getAddOnce(addOnce item) {
	return _addOnceMap[item];
}

bool Equip::canEffectOnce() {
	return _canEffectOnce;
}

void Equip::useOnce(void) {
	_canEffectOnce = false;
	return;
}

void Equip::setAttackArea(const AttackArea &a) {
	_attackArea = a;
	return;
}

AttackArea &Equip::getAttackArea() {
	return _attackArea;
}