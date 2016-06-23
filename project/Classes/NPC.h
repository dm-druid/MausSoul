#ifndef _NPC_H_
#define _NPC_H_

#include "Entity.h"

class NPC : public Entity {
public:
	NPC(void);
	~NPC(void) = default;

	CREATE_FUNC(NPC);
	virtual bool init(void);

	void addDialogue(const std::string &);
	void showDialogue(void);
	void hideDialogue(void);
	bool getIsTalking(void) { return _isTalking; }
private:
	cocos2d::Sprite *_dialogueSprite;
	bool _isTalking;
};


#endif