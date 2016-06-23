/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *  
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>
#include <string>
#include <memory>
#include <cstddef>

using std::size_t;
using std::ptrdiff_t;

constexpr size_t CHANNEL_R = 0;
constexpr size_t CHANNEL_G = 1;
constexpr size_t CHANNEL_B = 2;
constexpr size_t CHANNEL_A = 3;
constexpr size_t BITDEPTH = 32;

class PNG_Image {
public:
	/*
	 * image pixels are stored in the vector "Raw",
	 * 4 bytes per pixel, ordered RGBARGBA...
	 */

	std::shared_ptr<std::vector<unsigned char>> Raw;
	size_t DataWidth, DataHeight, Width, Height;
	size_t OffsetX, OffsetY;

	PNG_Image(std::shared_ptr<std::vector<unsigned char>> &raw,
		size_t DataWidth, size_t DataHeight, size_t Width, size_t Height,
		size_t OffsetX = 0, size_t OffsetY = 0)
		: Raw(raw), DataWidth(DataWidth), DataHeight(DataHeight), Width(Width), Height(Height),
		OffsetX(OffsetX), OffsetY(OffsetY) { }

	PNG_Image(std::shared_ptr<std::vector<unsigned char>> &raw, size_t Width, size_t Height)
		: PNG_Image(raw, Width, Height, Width, Height, 0, 0) { }

	// cut image constructor
	PNG_Image(PNG_Image &src, size_t Width, size_t Height, size_t OffsetX = 0, size_t OffsetY = 0)
		: PNG_Image(src.Raw, src.DataWidth, src.DataHeight, Width, Height,
			src.OffsetX + OffsetX, src.OffsetY + OffsetY) { }

	unsigned char &operator()(size_t x, size_t y, size_t rgbSel) {
		return (*Raw)[(y + OffsetY) * (DataWidth * BITDEPTH / 8)
			+ (x + OffsetX) * BITDEPTH / 8 + rgbSel];
	}

	const unsigned char &operator()(size_t x, size_t y, size_t rgbSel) const {
		return (*Raw)[(y + OffsetY) * (DataWidth * BITDEPTH / 8)
			+ (x + OffsetX) * BITDEPTH / 8 + rgbSel];
	}
};

std::shared_ptr<PNG_Image> readPNG(const std::string &filename);

#endif // _IMAGE_H_