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

#include "Image.h"
#include "lodepng.h"
#include <string>
#include <memory>
#include <sstream>

std::shared_ptr<PNG_Image> readPNG(const std::string &filename) {
	std::shared_ptr<std::vector<unsigned char>> image(new std::vector<unsigned char>());

	unsigned int width, height;
	unsigned int error = lodepng::decode(*image, width, height, filename.c_str(), LCT_RGBA);

	if (error) {
		std::stringstream ss;
		ss << "decoder error " << error << ": " << lodepng_error_text(error);
		throw ss.str();
	}

	std::shared_ptr<PNG_Image> PNG(new PNG_Image(PNG_Image(image, width, height, width, height)));
	return PNG;
}
