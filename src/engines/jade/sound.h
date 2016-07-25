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

#ifndef ENGINES_JADE_SOUND_H
#define ENGINES_JADE_SOUND_H

#include <src/common/types.h>
#include <src/common/ustring.h>

#include <src/engines/jade/object.h>

namespace Engines {

namespace Jade {

class Sound : public Object {
public:
	/** Load from a sound instance. */
	Sound(const Aurora::GFF3Struct &sound);
	~Sound();

	// Playing the sound

	void show(); ///< Start to play the sound cue associated with this sound.
	void hide(); ///< Stop playing the sound cue associated with this sound.

private:
	Common::UString _soundCue;  ///< The sound's sound cue.
	bool _isPlotObject;         ///< Does this sound represent a plot object?
	bool _isPriorityGroup;      ///< Is the sound in the priority group?
	float _volume;              ///< The volume (difference?) to play the sound cue with.

	/** Load from a sound instance. */
	void load(const Aurora::GFF3Struct &sound);
};

} // End of namespace Jade

} // End of namespace Engines

#endif // ENGINES_JADE_SOUND_H
