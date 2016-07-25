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
 *  A sound placeable in a Jade Empire area.
 */

#include <src/aurora/gff3file.h>

#include <src/engines/jade/sound.h>
#include <src/engines/jade/soundcueplayer.h>

namespace Engines {

namespace Jade {

Sound::Sound(const Aurora::GFF3Struct &sound) : Object(), _isPlotObject(0),
	_isPriorityGroup(0), _volume(0.0) {

	load(sound);
}

Sound::~Sound() {
}

void Sound::show() {
	CuePlay.playSoundCue3D(_soundCue, _position[0], _position[1], _position[2]);
}

void Sound::hide() {

}

void Sound::load(const Aurora::GFF3Struct &sound) {
	_tag = sound.getString("Tag", _tag);

	_active = sound.getBool("Active");

	_isPlotObject = sound.getBool("PlotObject");

	loadPositional(sound);

	_soundCue = sound.getString("SoundCue");

	_isPriorityGroup = sound.getBool("PriorityGroup");

	_volume = sound.getDouble("Volume");
}

} // End of namespace Jade

} // End of namespace Engines
