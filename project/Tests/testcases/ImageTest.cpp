/*
 * Project Pacific
 * Copyright (c) 2016 Pacific Development Team. All rights reserved.
 *
 * Unit test for Image class.
 *
 * written by SXKDZ
 */

#include "gtest/gtest.h"
#include "Image.h"
#include <string>

TEST(ImageTest, ImageReadBasicProperty) {
	auto png = readPNG(std::string("imageTest.png"));
	EXPECT_EQ(png.DataHeight, 680);
	EXPECT_EQ(png.DataWidth, 960);
}

TEST(ImageTest, ImageReadPixel) {
	auto png = readPNG(std::string("imageTest.png"));
	for (int i = 0; i < png.DataWidth; ++i) {
		for (int j = 0; j < png.DataHeight; ++j) {
			EXPECT_NEAR(png(i, j, CHANNEL_A), 255, 10);
		}
	}
}