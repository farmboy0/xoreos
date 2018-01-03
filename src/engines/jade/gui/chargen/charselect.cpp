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
 *  The character selection menu in the Jade character generation menu.
 */

#include "src/common/configman.h"

#include "src/aurora/2dareg.h"
#include "src/aurora/2dafile.h"
#include "src/aurora/talkman.h"

#include "src/engines/kotor/gui/widgets/kotorwidget.h"
#include "src/engines/kotor/gui/widgets/button.h"
#include "src/engines/kotor/gui/widgets/label.h"

#include "src/engines/kotor/gui/gui.h"

#include "src/engines/jade/gui/chargen/charselect.h"
#include "src/engines/jade/gui/chargen/chargeninfo.h"

namespace Engines {

namespace Jade {

CharacterSelection::CharacterSelection(CharacterGenerationInfo &UNUSED(info),
		Console *console) : ::Engines::KotOR::GUI(console), _currentPlayer(0) {

	load("char_select2");

	getLabel("LabelArrowLeft", true)->setClickable(true);
	getLabel("LabelArrowRight", true)->setClickable(true);
	getButton("bNameStr", true)->setClickable(false);

	getButton("ButtonMaster", true)->setDisabled(!ConfigMan.getBool("master"));

	update();
}

void CharacterSelection::callbackActive(Widget &widget) {
	if (widget.getTag() == "LabelArrowLeft") {
		const Aurora::TwoDAFile &_playerData = TwoDAReg.get2DA("players");
		if (_currentPlayer == 0) {
			_currentPlayer = _playerData.getRowCount();
		}
		_currentPlayer--;
		update();
	}
	if (widget.getTag() == "LabelArrowRight") {
		const Aurora::TwoDAFile &_playerData = TwoDAReg.get2DA("players");
		_currentPlayer++;
		if (_currentPlayer == _playerData.getRowCount()) {
			_currentPlayer = 0;
		}
		update();
	}
	if (widget.getTag() == "ButtonBack") {
		_returnCode = -1;
		return;
	}
	if (widget.getTag() == "ButtonTechnique") {
		_returnCode = 1;
		return;
	}
	if (widget.getTag() == "ButtonAccept") {
		_returnCode = 2;
		return;
	}
	if (widget.getTag() == "ButtonMaster") {
		_returnCode = 3;
		return;
	}
}

void CharacterSelection::update() {
	const Aurora::TwoDAFile &_playerData = TwoDAReg.get2DA("players");
	const Aurora::TwoDARow &selectedPlayer = _playerData.getRow(_currentPlayer);

	getButton("bNameStr", true)->setText(TalkMan.getString(selectedPlayer.getInt("name")));
}

} // End of namespace Jade

} // End of namespace Engines
