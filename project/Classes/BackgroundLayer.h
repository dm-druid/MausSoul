/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * written by SXKDZ, DM
 */

#ifndef _BACKGROUND_LAYER_H_
#define _BACKGROUND_LAYER_H_

#include "Image.h"
#include "Entity.h"
#include "Player.h"
#include "cocos2d.h"
#include "lodepng.h"
#include <string>
#include <vector>
#include <memory>
#include <cinttypes>

constexpr int SINGLE_MAP_WIDTH = 1920;
constexpr int SINGLE_MAP_HEIGHT = 1080;

struct BackgroundImages {
	std::string backgroundPath;
	std::string maskPath;
	cocos2d::Sprite *backgroundImage;
	std::shared_ptr<PNG_Image> maskImage;
	int column;
	int row;
	bool isShown;
};

class BackgroundLayer : public cocos2d::Layer {
public:
	BackgroundLayer(
		std::string baseBackgroundImgFilename,
		std::string baseMaskImgFilename,
		int rStart, int rEnd, int cStart, int cEnd);
	~BackgroundLayer(void);

	virtual bool init(void) override;
	static BackgroundLayer *create(
		std::string baseBackgroundImgFilename,
		std::string baseMaskImgFilename,
		int rStart, int rEnd, int cStart, int cEnd);

	void initBackground(void);
	void initSingleImage(int row, int column);
	void initSurroundingBackground(cocos2d::Point p);
	void setViewPointByPlayer(Player *);

	inline int getMapColumn(cocos2d::Point p);
	inline int getMapRow(cocos2d::Point p);
	bool isAccessible(cocos2d::Point p, Entity *e);

	void buildImagePaths(void);

private:
	std::vector<BackgroundImages> _backgroundImages;

	std::string _backgroundImageBaseFilename;
	std::string _maskImageBaseFilename;

	int _imageColumnStartPos;
	int _imageColumnEndPos;
	int _imageRowStartPos;
	int _imageRowEndPos;

	int _loadedCount;
};

#endif
