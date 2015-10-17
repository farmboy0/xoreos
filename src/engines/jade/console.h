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
 *  Jade (debug) console.
 */

#ifndef ENGINES_JADE_CONSOLE_H
#define ENGINES_JADE_CONSOLE_H

#include <vector>

#include "src/engines/aurora/console.h"

#include "src/engines/kotor/gui/gui.h"

namespace Engines {

namespace Jade {

class JadeEngine;

class Console : public ::Engines::Console {
public:
	Console(JadeEngine &engine);
	~Console();


private:
	JadeEngine *_engine;

	// Caches
	std::vector<Common::UString> _modules; ///< All known modules.


	// Updating the caches
	void updateCaches();
	void updateModules();

	// The commands
	void cmdExitModule (const CommandLine &cl);
	void cmdListModules(const CommandLine &cl);
	void cmdLoadModule (const CommandLine &cl);

	void cmdListXSB    (const CommandLine &cl);
	void cmdListCues   (const CommandLine &cl);
	void cmdListSets   (const CommandLine &cl);
	void cmdPlayCue    (const CommandLine &cl);
	void cmdPlaySetCue (const CommandLine &cl);
	void cmdMusicState (const CommandLine &cl);
	void cmdShowGUI (const CommandLine &cl);

	class GUILoader : public ::Engines::KotOR::GUI {
	public:
		GUILoader(const Common::UString &guiName, ::Engines::Console *console);
		~GUILoader();
	};
	GUILoader *_gui;
};

} // End of namespace Jade

} // End of namespace Engines

#endif // ENGINES_JADE_CONSOLE_H
