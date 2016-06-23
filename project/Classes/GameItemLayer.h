#ifndef _GAME_ITEM_LAYER_H_
#define _GAME_ITEM_LAYER_H_

#include "cocos2d.h"

class GameItemLayer : public cocos2d::Layer {
public:
	virtual bool init(void) override;
	CREATE_FUNC(GameItemLayer);

	void setHP(double percentage);
	void setExp(int exp);
	void setStamina(double percentage);
private:

	cocos2d::Sprite *_HPOuterHud;
	cocos2d::Sprite *_HPInnerHud;
	cocos2d::Sprite *_StaminaOuterHud;
	cocos2d::Sprite *_StaminaInnerHud;
	cocos2d::Vector<cocos2d::Sprite *> _PropBanner;
	cocos2d::Label *_ExpHud;
	cocos2d::Sprite *_ExpHudShadow;
	std::vector<cocos2d::Sprite *> _weaponBoxs;
	std::vector<cocos2d::Sprite *> _weapons;
};

#endif // !_GAME_ITEM_LAYER_H_