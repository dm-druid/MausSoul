/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by SXKDZ, DM
 */

#ifndef _EVENT_HANDLE_H_
#define _EVENT_HANDLE_H_

#include "NPC.h"
#include "Player.h"
#include "Entity.h"
#include "Monster.h"
#include "Substance.h"
#include "cocos2d.h"
#include <map>
#include <vector>

constexpr double INSCRIBED_ANGLE = 360.0;

class EventBase {
	// all the event are associated with the Player* and bind it
public:
	EventBase(void) { _isDisabled = false; };
	static void bind(Player *p) { player = p; }

	virtual bool verify(Entity *) = 0;
	virtual void execute(Entity *) = 0;

	bool isDisabled(void) { return _isDisabled; }
	void turnOn(void) { _isDisabled = false; }
	void turnOff(void) { _isDisabled = true; }
protected:
	static Player *player;
	bool _isDisabled;
};


class EventMonsterMark : public EventBase {
// when use this class, pass Monster pointer as the parameter
public:
	virtual bool verify(Entity *); 
	virtual void execute(Entity *);
}; 

class EventPassbyNPC : public EventBase {
// when use this class, pass Neutral pointer as the parameter
public:
	virtual bool verify(Entity *);
	virtual void execute(Entity *);
};

class EventCollision : public EventBase {
public:
	virtual bool verify(Entity *);
	virtual void execute(Entity *);
};

class EventAttack : public EventBase {
public:
	virtual bool verify(Entity *);
	virtual void execute(Entity *);
};

class EventMonsterAutomove : public EventBase {
public:
	virtual bool verify(Entity *);
	virtual void execute(Entity *);
};

class EventMonsterChase: public EventBase {
public:
	virtual bool verify(Entity *);
	virtual void execute(Entity *);
};

class EventMonsterAttack : public EventBase {
public:
	virtual bool verify(Entity *);
	virtual void execute(Entity *);
};

enum EventName : int {EVENT_MONSTER_MARK, EVENT_PASSBY_NPC, EVENT_COLLISION, EVENT_ATTACK,
						EVENT_MONSTER_AUTOMOVE, EVENT_MONSTER_CHASE, EVENT_MONSTER_ATTACK};
/*
 *still use enum as key to EventBase*
 * remember register with a Player*
 */
class EventHandle {
private:
	static std::vector<EventBase *> _eventList;
	static std::map<EventName, EventBase *> _eventNameMap;
public:
	EventHandle(void);
	~EventHandle(void);

	static void registerEventHandle(Player *);
	void eventSwitch(EventName, bool);
	void runEvent(Entity *);
};

#endif
