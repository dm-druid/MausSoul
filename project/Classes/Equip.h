/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by DM
 */

#ifndef _EQUIP_H_
#define _EQUIP_H_

#include "cocos2d.h"
#include "Body.h"
#include <map>
#include <string>

enum equipType : int {EQUIPTYPEBEGIN, SOMETHING, CLOTH, SHOES, WEAPON, EQUIPTYPEEND};
enum addOnce : int {ADDONCEBEGIN, ADDONCEHP, ADDONCEATK, ADDONCESPEED, ADDONCEDEFENCE, ADDONCEEND};
enum addValue : int {ADDVALUEBEGIN, ADDHP, ADDATK, ADDSPEED, ADDDEFENCE, ADDVALUEEND};

class Equip {
public:
	Equip(void);
	~Equip(void) = default;
	void setEquipType(equipType);
	equipType getEquipType(void);
	
	void setAddValue(addValue, cocos2d::Value);
	void setAddOnce(addOnce, cocos2d::Value);
	bool canEffectOnce(void);
	void useOnce(void);
	void setAttackArea(const AttackArea &);
	AttackArea &getAttackArea();

	cocos2d::Value getAddValue(addValue);
	cocos2d::Value getAddOnce(addOnce);

	void setName(const std::string &name) { equipName = name; }
	std::string getName() { return equipName; }

private:
	equipType _equipType;
	std::map<addValue, cocos2d::Value> _addMap;
	std::map<addOnce, cocos2d::Value> _addOnceMap;
	bool _canEffectOnce;
	AttackArea _attackArea;
	std::string equipName;
};

#endif /* _EQUIP_H_ */
