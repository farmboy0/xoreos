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

#include <algorithm>

#include <boost/bind.hpp>

#include "src/common/ustring.h"
#include "src/common/strutil.h"
#include "src/common/util.h"
#include "src/common/filepath.h"
#include "src/common/filelist.h"
#include "src/common/error.h"
#include "src/common/configman.h"

#include "src/aurora/resman.h"
#include "src/aurora/soundbank.h"
#include "src/aurora/types.h"

#include "src/graphics/aurora/types.h"

#include "src/engines/jade/console.h"
#include "src/engines/jade/jade.h"
#include "src/engines/jade/game.h"
#include "src/engines/jade/module.h"
#include "src/engines/jade/soundcueplayer.h"

namespace Engines {

namespace Jade {

Console::Console(JadeEngine &engine) :
	::Engines::Console(engine, Graphics::Aurora::kSystemFontMono, 13),
	_engine(&engine), _gui(0) {

	registerCommand("exitmodule" , boost::bind(&Console::cmdExitModule , this, _1),
			"Usage: exitmodule\nExit the module, returning to the main menu");
	registerCommand("listmodules", boost::bind(&Console::cmdListModules, this, _1),
			"Usage: listmodules\nList all modules");
	registerCommand("loadmodule" , boost::bind(&Console::cmdLoadModule , this, _1),
			"Usage: loadmodule <module>\nLoad and enter the specified module");

	registerCommand("listxsb" , boost::bind(&Console::cmdListXSB , this, _1),
			"Usage: listxsb\nList all sound bank names");
	registerCommand("listcues" , boost::bind(&Console::cmdListCues , this, _1),
			"Usage: listcues (<sound bank name>)\nList all sound cues (from a sound bank)");
	registerCommand("listsets" , boost::bind(&Console::cmdListSets , this, _1),
			"Usage: listsets\nList all sound sets");
	registerCommand("playcue" , boost::bind(&Console::cmdPlayCue , this, _1),
			"Usage: playcue <cue name> (sound index)\nPlay the specified sound cue (using sound at index)");
	registerCommand("playsetcue" , boost::bind(&Console::cmdPlaySetCue , this, _1),
			"Usage: playsetcue <sound set> <sound name>\nPlay the specified sound from the sound set");
	registerCommand("musicstate" , boost::bind(&Console::cmdMusicState , this, _1),
			"Usage: musicstate <new music state>\nSets the current music state");
	registerCommand("showgui" , boost::bind(&Console::cmdShowGUI , this, _1),
			"Usage: showgui <gui>\nLoad the GUI from the specified GUI resource");
}

Console::~Console() {
	if (_gui)
		delete _gui;
}

void Console::updateCaches() {
	::Engines::Console::updateCaches();

	updateModules();
}

void Console::updateModules() {
	_modules.clear();
	Game::getModules(_modules);
	_modules.push_back("aui1");
	_modules.push_back("aui2");
	_modules.push_back("aui3");
	_modules.push_back("aui4");
	_modules.push_back("aui5");
	_modules.push_back("aui6");

	setArguments("loadmodule", _modules);
}

void Console::cmdExitModule(const CommandLine &UNUSED(cl)) {
	hide();
	_engine->getGame().getModule().exit();
}

void Console::cmdListModules(const CommandLine &UNUSED(cl)) {
	updateModules();
	printList(_modules);
}

void Console::cmdLoadModule(const CommandLine &cl) {
	if (cl.args.empty()) {
		printCommandHelp(cl.cmd);
		return;
	}

	for (std::vector<Common::UString>::iterator m = _modules.begin(); m != _modules.end(); ++m) {
		if (m->equalsIgnoreCase(cl.args)) {
			hide();
			_engine->getGame().getModule().load(cl.args);
			return;
		}
	}

	printf("No such module \"%s\"", cl.args.c_str());
}

void Console::cmdListXSB(const CommandLine &cl) {
	if (!cl.args.empty()) {
		printCommandHelp(cl.cmd);
		return;
	}

	std::vector<Common::UString> names;
	CuePlay._cueSheet->getSoundBankNames(names);
	printList(names);
}

void Console::cmdListCues(const CommandLine &cl) {
	if (cl.args.contains(' ')) {
		printCommandHelp(cl.cmd);
		return;
	}

	std::vector<Common::UString> names;
	if (!cl.args.empty()) {
		CuePlay._cueSheet->getSoundCueNames(names, cl.args);
	} else {
		CuePlay._cueSheet->getSoundCueNames(names);
	}
	printList(names);
}

void Console::cmdListSets(const CommandLine &cl) {
	if (!cl.args.empty()) {
		printCommandHelp(cl.cmd);
		return;
	}

	std::list<Aurora::ResourceManager::ResourceID> resources;
	ResMan.getAvailableResources(Aurora::kFileTypeTXT, resources);

	std::vector<Common::UString> names;
	for (std::list<Aurora::ResourceManager::ResourceID>::const_iterator r = resources.begin(); r != resources.end(); ++r) {
		Common::UString name((*r).name);
		if (name.endsWith("_xsb")) {
			Common::UString path = ResMan.findResourceFile(name, Aurora::kFileTypeTXT);
			if (!path.empty() && path.contains("soundsets")) {
				name.erase(name.findFirst("_xsb"), name.end());
				names.push_back(name);
			}
		}
	}
	printList(names);
}

void Console::cmdPlayCue(const CommandLine &cl) {
	if (cl.args.empty()) {
		printCommandHelp(cl.cmd);
		return;
	}

	std::vector<Common::UString> args;
	Common::UString::split(cl.args, ' ', args);

	if (args.size() > 2) {
		printCommandHelp(cl.cmd);
		return;
	}

	try {
		if (args.size() == 1) {
			CuePlay.playSoundCue(args[0]);
		} else {
			uint32 soundIndex;
			Common::parseString(args[1], soundIndex);
			CuePlay.playSoundCue(args[0], soundIndex);
		}
	} catch (Common::Exception &e) {
		printException(e);
	}
}

void Console::cmdPlaySetCue(const CommandLine &cl) {
	if (cl.args.empty()) {
		printCommandHelp(cl.cmd);
		return;
	}

	std::vector<Common::UString> args;
	Common::UString::split(cl.args, ' ', args);

	if (args.size() != 2) {
		printCommandHelp(cl.cmd);
		return;
	}

	try {
		CuePlay.playSoundSetCue(args[0], args[1]);
	} catch (Common::Exception &e) {
		printException(e);
	}
}

void Console::cmdMusicState(const CommandLine &cl) {
	if (cl.args.empty() || cl.args.contains(' ')) {
		printCommandHelp(cl.cmd);
		return;
	}

	uint32 musicState;
	Common::parseString(cl.args, musicState);
	CuePlay.changeMusicState(musicState);
}

void Console::cmdShowGUI(const CommandLine &cl) {
	if (cl.args.empty()) {
		printCommandHelp(cl.cmd);
		return;
	}

	std::vector<Common::UString> args;
	Common::UString::split(cl.args, ' ', args);

	if (args.size() != 1) {
		printCommandHelp(cl.cmd);
		return;
	}

	try {
		if (_gui) {
			_gui->hide();
			delete _gui;
		}
		_gui = new GUILoader(args[0], this);
		_gui->show();
		hide();
	} catch (Common::Exception &e) {
		printException(e);
	}
}

Console::GUILoader::GUILoader(const Common::UString &guiName, ::Engines::Console *console = 0) :
	::Engines::KotOR::GUI(console) {

	load(guiName);
}
Console::GUILoader::~GUILoader() {
}

} // End of namespace Jade

} // End of namespace Engines
