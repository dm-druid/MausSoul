/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by nemoremold, DM
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "cocos2d.h"
#include "EntityProperties.h"
#include "AnimationSystem.h"
#include "NewSprite.h"
#include "GameItemLayer.h"
#include "Equip.h"
#include "Body.h"
#include <map>
#include <string>
#include <math.h>

enum EntityType : int { ENTITY, PLAYER, MONSTER, NEUTRAL, SUBSTANCE};

class Entity : public cocos2d::Node {
public:
	Entity(void);
	~Entity(void);
	NewSprite *getSprite(void);
	void bindSprite(NewSprite *sprite);
	void setEntityName(const std::string &);
	std::string &getEntityName();
	void setEntityType(EntityType);
	EntityType getEntityType(void);
	/*
	* getPosition:
	*  used to get the global coordinate of the entity, namely the position of the sprite bound to the entity;
	*  note that the position is not that of the entity object, but of the sprite bound to the entity;
	*  the position of the entity itself is on the lower-left corner of the scene;
	*  the position of the sprite object is the relative position of the entity object;
	*/
	virtual cocos2d::Point getPosition();
	double getOrientation();
	
	void setBody(Body);
	Body *getBody(void);
	void setAttackArea(AttackArea);
	AttackArea *getAttackArea(void);

	// interface function for objects to acquire their properties
	EntityProperties *getProperties(void);
	cocos2d::Point calcForwardPoint(void);
	cocos2d::Point calcBackwardPoint(void);
    
	bool wear(Equip *);
	bool takeoff(Equip *);
	bool takeoff(equipType);
	bool useEquipOnce(Equip *);

	// status settings
	/*
	 * set to decide the position of one entity initially,
	 * changing INITPOSTIONX and INITPOSITIONY of its properties
	 */
	void setInitialCoordinate(double, double);
	// used to set the default velocity of one entity, influencing current velocity
	void setSpeed(double);
	// set to decides how fast one entity sprints and moderates
	void setAcceleration(double);
	// angular velocity decides one entity's speed of rotating
	void setAngularSpeed(double);
	/*
	 * set the facing orientation of one entity where 0 refers to complete right,
	 * rotating clockwise, using angle
	 */
	void setOrientation(double);
	
	// basic active actions
	void moveForward(void);
	void moveBackward(void);
	void sprint(void);
	void rotateLeft(void);
	void rotateRight(void);
	void dodge(void);
	void castSpell(void);
	void useItem(void);
	void stopMovement(void);
	/*
	 * used for one entity to target on another,
	 * namely to lock one's camera on another and so on
	 */
	void target(Entity *target);
	void undoTargetting();

	// basic reactive actions
	void spawn(const cocos2d::Point &location);
	bool staminaEnough(void);
	void minusStamina(void);
	// for attack and defend
	void lossStamina(void);
	// for sprint
	void recoverStamina(void);
	void idle(void);
	void moderate(void);
	virtual void reset(void);
	void unguard(void);
	// used when one entity is being interacted to do some reactions
	void reactToInteraction(void);
	
	// compound actions
	// move forward by a distance
	void moveByDis(double dis);
	// rotate right by a degree
	void rotateByDegree(double degree);
	void patrol(void);
	void chase(void);
	void follow(void);

	void changeEquips();
	int getEquipNum() { return equipNum; }
	int getEquipMaxnum() { return _equips.size(); }

	void addEquip(Equip * _equip);
	
protected:
	cocos2d::Point _coordinate;
	Entity* _target;
	
	NewSprite *_sprite;
	std::string _name;
	EntityType _entityType;

	EntityProperties *_properties;
	std::map<equipType, Equip*> _equipMap;
	std::vector<Equip *> _equips;
	Equip *selectedEquip;

	Body _body;
	AttackArea _attackArea;

	int equipNum = 0;
};

#endif 
