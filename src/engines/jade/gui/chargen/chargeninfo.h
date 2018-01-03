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
 *  The class for storing character information for generation.
 */

#ifndef ENGINES_JADE_GUI_CHARGEN_CHARGENCHAR_H
#define ENGINES_JADE_GUI_CHARGEN_CHARGENCHAR_H

#include "src/common/ustring.h"

#include "src/graphics/aurora/model.h"

#include "src/engines/jade/types.h"
#include "src/engines/jade/creature.h"

namespace Engines {

namespace Jade {

class CharacterGenerationInfo {
public:
	CharacterGenerationInfo();

	/** Get the name of the character. */
	const Common::UString &getName();
	/** Get the name of the portrait of this character. */
	Common::UString getPortrait();

	/** Set the name of the Character. */
	void setName(const Common::UString &name);

	Creature *getCharacter();
	Graphics::Aurora::Model *getModel();

private:
	Common::UString _name;

	Common::ScopedPtr<Graphics::Aurora::Model> _body;
};

} // End of namespace Jade

} // End of namespace Engines

#endif // ENGINES_JADE_GUI_CHARGEN_CHARGENCHAR_H
