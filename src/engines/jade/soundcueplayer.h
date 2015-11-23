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
 *  A class dealing with playing sound cues.
 */

#ifndef ENGINES_JADE_SOUNDCUEPLAYER_H
#define ENGINES_JADE_SOUNDCUEPLAYER_H

#include "src/common/singleton.h"
#include "src/common/types.h"

#include "src/aurora/soundbank.h"
#include "src/aurora/wavebank.h"

#include "src/sound/sound.h"

namespace Common {
	class UString;
}

namespace Engines {

namespace Jade {

class Console;
class Object;

class SoundCuePlayer : public Common::Singleton<SoundCuePlayer> {

	friend class Engines::Jade::Console;

public:
	SoundCuePlayer();
	~SoundCuePlayer();

	/** Plays the sound cue with the given name. */
	void playSoundCue(const Common::UString &soundCueName);

	/** Plays the sound cue with the given name in 3D at the given position. */
	void playSoundCue3D(const Common::UString &soundCueName, float x, float y, float z);

	/** Plays the sound cue with the given name using the sound at the given index. */
	void playSoundCue(const Common::UString &soundCueName, const uint32 soundIndex);

	/** Plays the sound with the given name from the given sound set. */
	void playSoundSetCue(const Common::UString &soundSet, const Common::UString &soundRef);

	/** Changes the state of music channel which defines which music to play. */
	void changeMusicState(const uint32 musicState);

	/** Returns the State of the currently playing music track. */
	uint32 getMusicState() const;
private:
	/** The cue sheet containing the sound cues for Jade Empire. */
	Aurora::CueSheet *_cueSheet;

	typedef std::map<Common::UString, Aurora::SoundBank> SoundBankMap;
	typedef std::map<Common::UString, Aurora::WaveBank> WaveBankMap;

	/** Map of sound bank name to sound bank. */
	SoundBankMap _soundbankMap;

	/** Map of wave bank name to wave bank. */
	WaveBankMap _wavebankMap;

	uint32 _musicState;

	::Sound::ChannelHandle _soundChannel;
	::Sound::ChannelHandle _musicChannel;

	/** Plays the given sound cue. */
	void playSoundCue(Aurora::SoundBank &soundbank, const Common::UString &soundCueName, uint32 variable = 0);

	/** Plays the given sound. */
	void playSound(Aurora::Sound &sound);

	/** Plays the given sound as music. */
	void playMusic(Aurora::Sound &sound);

	/** Select a sound to play from the sound cue. */
	Aurora::Sound &chooseSound(Aurora::SoundBank &soundbank, const Aurora::SoundCue &soundCue, uint32 variable = 0);

	/** Select a sound resource to play from the sound. */
	const Common::UString &chooseSoundResource(Aurora::Sound &sound);

	Aurora::WaveBank  &fetchWaveBank (const Common::UString &wavebankName);
	Aurora::SoundBank &fetchSoundBank(const Common::UString &soundbankName);
};

} // End of namespace Jade

} // End of namespace Engines


/** Shortcut for accessing the sound cue player. */
#define CuePlay ::Engines::Jade::SoundCuePlayer::instance()

#endif // ENGINES_JADE_SOUNDCUEPLAYER_H
