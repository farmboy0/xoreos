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

#ifndef ENGINES_JADE_GUI_INGAME_INGAME_H
#define ENGINES_JADE_GUI_INGAME_INGAME_H

#include <vector>

#include "src/common/types.h"

#include "src/events/types.h"

namespace Common {
	class UString;
}

namespace Engines {

namespace Jade {

class Module;

class Object;
class Creature;

struct ChapterScreenInfo;

/** The Jade Empire ingame GUI elements. */
class IngameGUI {
public:
	IngameGUI(Module &module, ::Engines::Console *console = 0);
	~IngameGUI();

	void show(); ///< Show the ingame GUI elements.
	void hide(); ///< Hide the ingame GUI elements.

	void addEvent(const Events::Event &event);
	void processEventQueue();

	/** Do we have a currently running conversation? */
	bool hasRunningConversation() const;

	/** Start a conversation. */
	bool startConversation(const Common::UString &conv, Creature &pc, Object &obj,
	                       const bool noWidescreen, const bool resetZoom);

	/** Stop a conversation. */
	void stopConversation();

	/** Show the chapter screen. */
	void showChapterScreen(ChapterScreenInfo *chapter);

private:
	Module *_module;
};

} // End of namespace Jade

} // End of namespace Engines

#endif // ENGINES_JADE_GUI_INGAME_INGAME_H
