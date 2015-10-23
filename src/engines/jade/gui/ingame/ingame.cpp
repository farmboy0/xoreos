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
 *  The Jade Empire ingame GUI elements.
 */

#include <cassert>

#include "src/common/error.h"

#include "src/engines/jade/module.h"
#include "src/engines/jade/gui/ingame/ingame.h"
#include "src/engines/jade/gui/ingame/chapter.h"

namespace Engines {

namespace Jade {

IngameGUI::IngameGUI(Module &module, ::Engines::Console *UNUSED(console)) :
	_module(&module), _chapterScreen(0) {

}

IngameGUI::~IngameGUI() {
}

void IngameGUI::show() {
}

void IngameGUI::hide() {
}

void IngameGUI::addEvent(const Events::Event &event) {
	if (_chapterScreen) {
		_chapterScreen->addEvent(event);
		return;
	}
}

void IngameGUI::processEventQueue() {
}

bool IngameGUI::hasRunningConversation() const {
	return false;
}

bool IngameGUI::startConversation(const Common::UString &UNUSED(conv), Creature &UNUSED(pc), Object &UNUSED(obj),
                                  const bool UNUSED(noWidescreen), const bool UNUSED(resetZoom)) {

	return false;
}

void IngameGUI::stopConversation() {
}

void IngameGUI::showChapterScreen(ChapterScreenInfo *chapter) {
	_chapterScreen = new ChapterScreen(chapter);
	_chapterScreen->show();
	_chapterScreen->run();
	_chapterScreen->hide();

	delete _chapterScreen;

	_chapterScreen = 0;
}

} // End of namespace Jade

} // End of namespace Engines
