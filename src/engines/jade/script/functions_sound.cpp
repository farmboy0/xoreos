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
 *  Jade Empire engine functions handling sound.
 */

#include "src/aurora/2dareg.h"
#include "src/aurora/2dafile.h"
#include "src/aurora/talkman.h"
#include "src/aurora/ssffile.h"

#include "src/aurora/nwscript/functioncontext.h"

#include "src/engines/jade/objectcontainer.h"
#include "src/engines/jade/object.h"
#include "src/engines/jade/creature.h"
#include "src/engines/jade/area.h"
#include "src/engines/jade/soundcueplayer.h"

#include "src/engines/jade/script/functions.h"

namespace Engines {

namespace Jade {

void Functions::playSound2D(Aurora::NWScript::FunctionContext &ctx) {
	Common::UString &soundName = ctx.getParams()[0].getString();

	CuePlay.playSoundCue(soundName);
}

void Functions::playSound3D(Aurora::NWScript::FunctionContext &ctx) {
	Common::UString &soundName = ctx.getParams()[0].getString();

	Jade::Object *caller = Jade::ObjectContainer::toObject(ctx.getCaller());
	if (!caller)
		return;

	float x, y, z;
	caller->getPosition(x, y, z);

	// TODO 3D
	CuePlay.playSoundCue(soundName);
}

void Functions::playSoundAtPosition(Aurora::NWScript::FunctionContext &ctx) {
	Common::UString &soundName = ctx.getParams()[0].getString();

	float x, y, z;
	ctx.getParams()[1].getVector(x, y, z);

	//TODO 3D
	CuePlay.playSoundCue(soundName);
}

void Functions::playSoundSet(Aurora::NWScript::FunctionContext &ctx) {
	Common::UString &soundName = ctx.getParams()[0].getString();

	Jade::Creature *creature = Jade::ObjectContainer::toCreature(ctx.getCaller());
	if (!creature)
		return;

	float x, y, z;
	creature->getPosition(x, y, z);

	const Aurora::TwoDARow &appearance = TwoDAReg.get2DA("appearance").getRow(creature->getAppearance());
	const Common::UString &soundSet = appearance.getString("DefaultSoundSet");

	//TODO 3D
	CuePlay.playSoundSetCue(soundSet, soundName);
}

void Functions::soundObjectPlay(Aurora::NWScript::FunctionContext &ctx) {
	Object *sound = Jade::ObjectContainer::toObject(getParamObject(ctx, 0));
	if (sound)
		sound->show();
}

void Functions::soundObjectStop(Aurora::NWScript::FunctionContext &ctx) {
	Object *sound = Jade::ObjectContainer::toObject(getParamObject(ctx, 0));
	if (sound)
		sound->hide();
}

void Functions::pauseSound(Aurora::NWScript::FunctionContext &ctx) {
	int32 pauseAll = ctx.getParams()[0].getInt();
	SoundMan.pauseAll(pauseAll);
}

void Functions::disableSoundSet(Aurora::NWScript::FunctionContext &ctx) {
	unimplementedFunction(ctx);
}

void Functions::disableMusicStateChanges(Aurora::NWScript::FunctionContext &ctx) {
	unimplementedFunction(ctx);
}

void Functions::setMusicState(Aurora::NWScript::FunctionContext &ctx) {
	uint32 musicState = (uint32) ctx.getParams()[0].getInt();

	CuePlay.changeMusicState(musicState);
}

void Functions::getMusicState(Aurora::NWScript::FunctionContext &ctx) {
	ctx.getReturn() = (int32) CuePlay.getMusicState();
}

void Functions::setAreaMusicState(Aurora::NWScript::FunctionContext &ctx) {
	unimplementedFunction(ctx);
}

} // End of namespace Jade

} // End of namespace Engines
