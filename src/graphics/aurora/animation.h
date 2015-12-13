/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  An animation to be applied to a model.
 */

#ifndef GRAPHICS_AURORA_ANIMATION_H
#define GRAPHICS_AURORA_ANIMATION_H

#include <list>
#include <map>

#include "src/common/ustring.h"
#include "src/common/transmatrix.h"
#include "src/common/boundingbox.h"

#include "src/graphics/types.h"
#include "src/graphics/glcontainer.h"
#include "src/graphics/renderable.h"

#include "src/graphics/aurora/types.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {

namespace Aurora {

class ModelNode;

class Animation {
public:
	Animation();
	~Animation();

	/** Get the animation's name. */
	const Common::UString &getName() const;
	void setName(Common::UString &name);

	/** Get the animations length. */
	float getLength() const;
	void setLength(float length);

	void setTransTime(float transtime);

	void addModelNode(ModelNode *node);

	/** Update the model position and orientation */
	void update(Model *model, float lastFrame, float nextFrame);

protected:
	typedef std::list<ModelNode *> NodeList;
	typedef std::map<Common::UString, ModelNode *, Common::UString::iless> NodeMap;

	NodeList nodeList; ///< The nodes within the animation.
	NodeMap  nodeMap;  ///< The nodes within the animation, indexed by name.

	Common::UString _name; ///< The model's name.
	float _length;
	float _transtime;

private:
	// Animation helpers
	void interpolatePosition(ModelNode * animNode, ModelNode *target, float time, float scale) const;
	void interpolateOrientation(ModelNode * animNode, ModelNode *target, float time) const;
};

} // End of namespace Aurora

} // End of namespace Graphics

#endif // GRAPHICS_AURORA_ANIMATION_H
