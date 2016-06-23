/*
* Project Pacific
* Copyright (c) 2016 Pacific Development Team. All rights reserved.
*
* written by SXKDZ, DM
*/

#include "Body.h"
#include "BackgroundLayer.h"
#include "cocos2d.h"
#include "lodepng.h"
#include <string>
#include <vector>
#include <thread>
#include <cstring>
#include <sstream>
#include <iostream>
#include <cinttypes>

BackgroundLayer *BackgroundLayer::create(
	std::string baseBackgroundImgFilename,
	std::string baseMaskImgFilename,
	int rStart, int rEnd, int cStart, int cEnd) {
	BackgroundLayer *pRet = new(std::nothrow) BackgroundLayer(
		baseBackgroundImgFilename, baseMaskImgFilename,
		rStart, rEnd, cStart, cEnd);
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
	}
	return nullptr;
}

/*
* Usage:
* When the background image is divided into several columns and 2 rows,
* e.g. image2.1.1.png image2.1.2.png ... image2.1.9.png
*      image2.2.1.png image2.2.2.png ... image2.2.9.png
*  and coverImage2.1.1.png coverImage2.1.2.png ... coverImage2.1.9.png
*      coverImage2.2.1.png coverImage2.2.2.png ... coverImage2.2.9.png
*
* In this case, you should give "image2.%c.%r.png", where
* "%c" indicates the column and "%r" indicates the row
*
* The second, third, fourth and fifth parameter indicate
* the start and end of the auto-increasing number of the image file
* of rows and columns respectively.
*
* In this case, you should call that function like:
* BackgroundLayer("image2.%c.%r.png", "coverImage2.%c.%r.png", 1, 9, 1, 2)
*/

BackgroundLayer::BackgroundLayer(
	std::string baseBackgroundImgFilename,
	std::string baseMaskImgFilename,
	int rStart, int rEnd, int cStart, int cEnd) :

	_backgroundImageBaseFilename(baseBackgroundImgFilename),
	_maskImageBaseFilename(baseMaskImgFilename),
	_imageRowStartPos(rStart),
	_imageRowEndPos(rEnd),
	_imageColumnStartPos(cStart),
	_imageColumnEndPos(cEnd) {
	return;
}

BackgroundLayer::~BackgroundLayer(void) {
	return;
}

bool BackgroundLayer::init(void) {
	if (!Layer::init()) {
		return false;
	}

	initBackground();
	return true;
}

void BackgroundLayer::buildImagePaths(void) {
	auto columnCount = _imageColumnEndPos - _imageColumnStartPos + 1;
	auto rowCount = _imageRowEndPos - _imageRowStartPos + 1;

	if (columnCount <= 0 || rowCount <= 0) {
		throw std::string("background image index error");
	}

	std::stringstream ss;
	std::string index;
	auto getIndexStringFromInt = [&](int i) { ss.clear(); ss << i; ss >> index; };
	auto replaceStringPlaceholders =
		[&](std::string &s, std::string placeholder, std::string index) {
		s.replace(s.find(placeholder), placeholder.size(), index);
	};

	// build image paths for required images from columns to rows
	for (int i = _imageColumnStartPos; i <= _imageColumnEndPos; ++i) {
		auto backgroundImagePath = _backgroundImageBaseFilename;
		auto maskImagePath = _maskImageBaseFilename;
		getIndexStringFromInt(i);
		replaceStringPlaceholders(backgroundImagePath, std::string("%c"), index);
		replaceStringPlaceholders(maskImagePath, std::string("%c"), index);

		for (int j = _imageRowStartPos; j <= _imageRowEndPos; ++j) {
			// for every loop of row, s_new should remain like "image1.1.%r.png"
			auto backgroundImagePathNew = backgroundImagePath;
			auto maskImagePathNew = maskImagePath;

			getIndexStringFromInt(j);
			replaceStringPlaceholders(backgroundImagePathNew, std::string("%r"), index);
			replaceStringPlaceholders(maskImagePathNew, std::string("%r"), index);

			BackgroundImages bi;
			bi.column = i;
			bi.row = j;
			bi.backgroundPath = backgroundImagePathNew;
			bi.maskPath = maskImagePathNew;
			bi.isShown = false;
			_backgroundImages.push_back(bi);
		}
	}
	return;
}

void BackgroundLayer::initSingleImage(int row, int column) {
	if (_loadedCount == 0) {
		return;
	}
	auto countColumn = _imageColumnEndPos - _imageColumnStartPos + 1;
	auto countRow = _imageRowEndPos - _imageRowStartPos + 1;
	auto flag = true;

	for (auto &bi : _backgroundImages) {
		if (!bi.isShown) {
			flag = false;
		}
		if (row == bi.row && column == bi.column) {
			if (!bi.isShown) {
				--_loadedCount;

				auto *sprite = cocos2d::Sprite::create(bi.backgroundPath);
				sprite->setAnchorPoint(cocos2d::Vec2::ZERO);
				sprite->setPosition(
					cocos2d::Vec2((bi.column - 1) * SINGLE_MAP_WIDTH,
					(bi.row - 1) * SINGLE_MAP_HEIGHT));
				sprite->setTag((bi.row - 1) * countRow + bi.column);
				addChild(sprite);

				auto readPNGThread = [&]() {bi.maskImage = readPNG(bi.maskPath);};
				std::thread th = std::thread(readPNGThread);
				th.detach();

				bi.backgroundImage = sprite;
				bi.isShown = true;

				break;
			}
		}
	}

	return;
}

void BackgroundLayer::initSurroundingBackground(cocos2d::Point p) {
	auto currentColumn = getMapColumn(p);
	auto currentRow = getMapRow(p);
	initSingleImage(currentRow, currentColumn);

	constexpr short direction[8][2] = {
		{ 0, 1 },{ 0, -1 },{ 1, 0 },{ -1, 0 },
		{ 1, 1 },{ -1, -1 },{ 1, -1 },{ -1, 1 } };
	for (auto i = 0; i < 8; ++i) {
		initSingleImage(currentRow + direction[i][0],
			currentColumn + direction[i][1]);
	}
	return;
}

void BackgroundLayer::initBackground(void) {
	_loadedCount =
		(_imageColumnEndPos - _imageColumnStartPos + 1) * (_imageRowEndPos - _imageRowStartPos + 1);
	buildImagePaths();
	return;
}

void BackgroundLayer::setViewPointByPlayer(Player *player) {
	auto pos = player->getPosition();

	/*
	* for simpilication, we consider that the movement of the player
	* will only be related with the movement of each image of columns
	*/
	int countColumn = _imageColumnEndPos - _imageColumnStartPos + 1;
	int countRow = _imageRowEndPos - _imageRowStartPos + 1;

	auto parent = player->getParent();
	auto viewSize = parent->getContentSize();

	if (pos.x >= viewSize.width / 2 && pos.x <= viewSize.width * (countColumn - 0.5)) {
		parent->setPositionX(viewSize.width / 2 - pos.x);
	}
	if (pos.y >= viewSize.height / 2 && pos.y <= viewSize.height * (countRow - 0.5)) {
		parent->setPositionY(viewSize.height / 2 - pos.y);
	}
	return;
}

inline int BackgroundLayer::getMapColumn(cocos2d::Point p) {
	/*
	* bug fixed: pixels are from 0 to weight / height - 1,
	* when the point is (1920, 1080), it should belong to
	* column 2, row 2
	*/
	return static_cast<int>(p.x / SINGLE_MAP_WIDTH) + 1;
}

inline int BackgroundLayer::getMapRow(cocos2d::Point p) {
	return static_cast<int>(p.y / SINGLE_MAP_HEIGHT) + 1;
}

// p is a world position of the background
bool BackgroundLayer::isAccessible(cocos2d::Point p, Entity *e) {
	// to test whether a point is accessible, namely whether one entity can be put onto

	/*
	* to get the reference of all pixels in the current image
	* the formula of getting the location of _maskLayerPixels is (c - 1) * colCount + r
	* here the subscript of at() function of vector start from 0
	*/
	cocos2d::Point originalPoint;
	std::vector<double> angleVec = e->getBody()->angle;
	std::vector<double> radiusVec = e->getBody()->radius;
	bool flag = true;
	double orientation = e->getProperties()->getValue(ORIENTATION).asFloat() / 180 * M_PI;
	auto angleVecBeg = angleVec.begin();
	auto angleVecEnd = angleVec.end();
	auto radiusVecBeg = radiusVec.begin();
	auto radiusVecEnd = radiusVec.end();

	/*
	* We have 12 points around the entity to judge
	* wheather it is out of the range of the accessible area.
	* -1 * M_PI / 6, 0, M_PI / 6, -1 * M_PI / 3, M_PI / 3,
	* -1 * M_PI / 2, M_PI / 2, -1 * M_PI *2 / 3, M_PI * 2 / 3,
	* -1 * M_PI * 5 / 6, M_PI * 5 / 6, M_PI
	* the angle between each vector of the 12 points with the postion point is 30 degrees
	* If the entity is moving forward, use the fisrt 3 points
	* And if moving backward, use the last 3 points
	*/
	if (e->getProperties()->getValue(ISMOVINGFORWARD).asBool()) {
		angleVecEnd = angleVec.begin() + 3;
		radiusVecEnd = radiusVec.begin() + 3;
		//p += e->calcForwardPoint();
	}
	if (e->getProperties()->getValue(ISMOVINGBACKWARD).asBool()) {
		angleVecBeg = angleVec.end() - 3;
		radiusVecBeg = radiusVec.end() - 3;
		//p += e->calcBackwardPoint();
	}

	for (; angleVecBeg != angleVecEnd; ++angleVecBeg, ++radiusVecBeg) {

		if (e->getProperties()->getValue(ISROTATING).asBool()) {
			if (e->getProperties()->getValue(ROTATINGLEFT).asBool()) {
				*angleVecBeg -= e->getProperties()->getValue(ANGULARVELOCITY).asDouble() * M_PI / 180;
			}
			else if (e->getProperties()->getValue(ROTATINGRIGHT).asBool()) {
				*angleVecBeg += e->getProperties()->getValue(ANGULARVELOCITY).asDouble() * M_PI / 180;
			}
		}

		cocos2d::Point pCur = p + cocos2d::Vec2(*radiusVecBeg * cos(orientation + *angleVecBeg),
			*radiusVecBeg * (-1) * sin(orientation + *angleVecBeg));

		auto currentColumn = getMapColumn(pCur);
		auto currentRow = getMapRow(pCur);

		originalPoint.x = pCur.x - (currentColumn - 1) * SINGLE_MAP_WIDTH;
		originalPoint.y = SINGLE_MAP_HEIGHT - (pCur.y - (currentRow - 1) * SINGLE_MAP_HEIGHT) - 1;

		auto currentPNG = _backgroundImages.begin()->maskImage;
		for (auto &bi : _backgroundImages) {
			if (bi.column == currentColumn && bi.row == currentRow) {
				currentPNG = bi.maskImage;
				break;
			}
		}

		auto currentAlpha = (*currentPNG)(static_cast<size_t>(originalPoint.x),
			static_cast<size_t>(originalPoint.y), CHANNEL_A);

		if (currentAlpha != 255) {
			flag = false;
		}
	}

	return flag;
}