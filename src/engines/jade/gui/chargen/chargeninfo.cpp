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

#include "src/common/strutil.h"
#include "src/common/error.h"

#include "src/engines/aurora/model.h"

#include "src/engines/jade/gui/chargen/chargeninfo.h"

namespace Engines {

namespace Jade {

CharacterGenerationInfo::CharacterGenerationInfo() : _body(0) {

}

const Common::UString &CharacterGenerationInfo::getName() {
	return _name;
}

Common::UString CharacterGenerationInfo::getPortrait() {
	Common::UString portrait = "po_p";

	return portrait;
}

void CharacterGenerationInfo::setName(const Common::UString &name) {
	_name = name;
}

Creature *CharacterGenerationInfo::getCharacter() {
	Creature *creature = new Creature();
//	creature->createPC(this);
	return creature;
}

Graphics::Aurora::Model *CharacterGenerationInfo::getModel() {
	if (_body)
		return _body.get();

	Common::UString body, head;

	body = "p";
	head = "p";

	body += "bb";
	head += "h";


	_body.reset(loadModelObject(body, ""));


	head += "0";

	Graphics::Aurora::Model *headModel = loadModelObject(head, "");
	_body->attachModel("headhook", headModel);

	return _body.get();
}

} // End of namespace Jade

} // End of namespace Engines

