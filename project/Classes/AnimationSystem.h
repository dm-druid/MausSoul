/*
* Project Pacific
* Copyright (c) 2016 Pacific Development Team. All rights reserved.
*
* written by nemoremold
*/

#ifndef _ANIMATIONSYSTEM_H_
#define _ANIMATIONSYSTEM_H_

#include <string>
#include "cocos2d.h"

class Anim : public cocos2d::Animation {
public:
	/*
	* Two types of create ways:
	* the first one's animation returns to the start frame automatically
	* while the second one is decided by user
	*/
	static cocos2d::Animation *createAnimation(
		std::string root, std::string target, std::string animation,
		int start, int end, double delay);
	static cocos2d::Animation *createAnimation(
		std::string root, std::string target, std::string animation,
		int start, int end, double delay, bool);
};

#endif
