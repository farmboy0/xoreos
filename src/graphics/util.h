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
 * The Infinity, Aurora, Odyssey, Eclipse and Lycium engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

/** @file graphics/util.h
 *  Graphics related utility functions.
 */

#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreThreadHeaders.h>

#include "common/types.h"
#include "common/util.h"
#include "common/ustring.h"
#include "common/maths.h"

namespace Graphics {

static inline Ogre::SceneManager &getOgreSceneManager(const Common::UString &name = "world") {
	Ogre::SceneManager *scene = Ogre::Root::getSingleton().getSceneManager(name.c_str());
	assert(scene);
	return *scene;
}

/** Flip an image horizontally. */
static inline void flipHorizontally(byte *data, int width, int height, int bpp) {
	int halfWidth = width / 2;
	int pitch     = bpp * width;

	byte *buffer = new byte[bpp];

	while (height-- > 0) {
		byte *dataStart = data;
		byte *dataEnd   = data + pitch - bpp;

		for (int j = 0; j < halfWidth; j++) {
			memcpy(buffer   , dataStart, bpp);
			memcpy(dataStart, dataEnd  , bpp);
			memcpy(dataEnd  , buffer   , bpp);

			dataStart += bpp;
			dataEnd   -= bpp;
		}

		data += pitch;
	}

	delete[] buffer;
}

/** Flip an image vertically. */
static inline void flipVertically(byte *data, int width, int height, int bpp) {
	int halfHeight = height / 2;
	int pitch      = bpp * width;

	byte *dataStart = data;
	byte *dataEnd   = data + (pitch * height) - pitch;

	byte *buffer = new byte[pitch];

	while (halfHeight--) {
		memcpy(buffer   , dataStart, pitch);
		memcpy(dataStart, dataEnd  , pitch);
		memcpy(dataEnd  , buffer   , pitch);

		dataStart += pitch;
		dataEnd   -= pitch;
	}

	delete[] buffer;
}

/** De-"swizzle" a texture pixel offset. */
static inline uint32 deSwizzleOffset(uint32 x, uint32 y, uint32 width, uint32 height) {
	width  = Common::intLog2(width);
	height = Common::intLog2(height);

	uint32 offset     = 0;
	uint32 shiftCount = 0;

	while (width | height) {
		if (width) {
			offset |= (x & 0x01) << shiftCount;

			x >>= 1;

			shiftCount++;
			width--;
		}

		if (height) {
			offset |= (y & 0x01) << shiftCount;

			y >>= 1;

			shiftCount++;
			height--;
		}
	}

	return offset;
}

} // End of namespace Graphics

/** Macro to lock the SceneManager / frame rendering for the remainder of the current scope.
    Note: To avoid deadlocks, never use the RequestManager to force a function to run in the
          main thread after the SceneManager has been locked! */
#define LOCK_FRAME() OGRE_LOCK_MUTEX(Graphics::getOgreSceneManager().sceneGraphMutex)

#endif // GRAPHICS_UTIL_H
