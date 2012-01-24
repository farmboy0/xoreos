/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

/** @file graphics/images/winiconimage.h
 *  Decoding Windows icon and cursor files (.ICO and .CUR).
 */

#ifndef GRAPHICS_IMAGES_WINICONIMAGE_H
#define GRAPHICS_IMAGES_WINICONIMAGE_H

#include "graphics/images/decoder.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {

/** Windows cursor. */
class WinIconImage : public ImageDecoder {
public:
	WinIconImage(Common::SeekableReadStream &cur);
	~WinIconImage();

	int getHotspotX() const;
	int getHotspotY() const;

private:
	uint16 _imageCount;
	uint16 _iconType;

	int _hotspotX;
	int _hotspotY;

	// Loading helpers
	void load(Common::SeekableReadStream &cur);
	void readHeader(Common::SeekableReadStream &cur);
	void readData(Common::SeekableReadStream &cur);
};

} // End of namespace Graphics

#endif // GRAPHICS_IMAGES_WINICONIMAGE_H
