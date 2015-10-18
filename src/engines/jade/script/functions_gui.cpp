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
 *  Jade Empire engine functions dealing with the gui.
 */

#include "src/common/util.h"

#include "src/aurora/talkman.h"

#include "src/aurora/nwscript/functioncontext.h"

#include "src/engines/jade/game.h"
#include "src/engines/jade/module.h"
#include "src/engines/jade/types.h"

#include "src/engines/jade/script/functions.h"

namespace Engines {

namespace Jade {

void Functions::showChapterScreen(Aurora::NWScript::FunctionContext &ctx) {
	ChapterScreenInfo chapter;
	chapter.title       = TalkMan.getString(ctx.getParams()[0].getInt());
	chapter.text        = TalkMan.getString(ctx.getParams()[1].getInt());
	chapter.sound       = ctx.getParams()[2].getString();
	chapter.scrollSpeed = ctx.getParams()[3].getFloat();
	chapter.isSkippable = ctx.getParams()[4].getInt() != 0;
	chapter.background  = ctx.getParams()[5].getString();

	chapter.useTitleColor = ctx.getParams()[6].getInt() != 0;
	ctx.getParams()[7].getVector(chapter.titleColorR, chapter.titleColorG, chapter.titleColorB);

	chapter.useTextColor = ctx.getParams()[8].getInt() != 0;
	ctx.getParams()[9].getVector(chapter.textColorR, chapter.textColorG, chapter.textColorB);

	chapter.useGUI = ctx.getParams()[10].getInt() != 0;
	chapter.gui    = ctx.getParams()[11].getString();

	_game->getModule().showChapterScreen(&chapter);
}

} // End of namespace Jade

} // End of namespace Engines
