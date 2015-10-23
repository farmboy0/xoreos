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
 *  The Jade Empire chapter screen.
 */

#include "src/common/error.h"
#include "src/common/util.h"

#include "src/engines/aurora/widget.h"

#include "src/engines/kotor/gui/widgets/kotorwidget.h"
#include "src/engines/kotor/gui/widgets/label.h"

#include "src/engines/jade/gui/ingame/chapter.h"

#include "src/graphics/graphics.h"

namespace Engines {

namespace Jade {

ChapterScreen::ChapterScreen(ChapterScreenInfo *chapter, ::Engines::Console *console) :
	::Engines::KotOR::GUI(console) {

	if (!chapter) {
		throw Common::Exception("Error initializing chapter screen.");
	}

	if (chapter->useGUI) 
		load(chapter->gui);
	else
		load("chapscreen");

	getLabel("TitleLabel", true)->setText(chapter->title);
	getLabel("LabelText", true)->setText(chapter->text);
}

ChapterScreen::~ChapterScreen() {
}

void ChapterScreen::callbackActive(Widget &UNUSED(widget)) {
	_returnCode = 1;
}

} // End of namespace Jade

} // End of namespace Engines
