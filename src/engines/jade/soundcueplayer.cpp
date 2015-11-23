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

#include "src/common/error.h"
#include "src/common/ustring.h"

#include "src/aurora/2dafile.h"
#include "src/aurora/2dareg.h"
#include "src/aurora/resman.h"
#include "src/aurora/types.h"

#include "src/sound/types.h"

#include "src/engines/aurora/util.h"

#include "src/engines/jade/soundcueplayer.h"

DECLARE_SINGLETON(Engines::Jade::SoundCuePlayer)

namespace Engines {

namespace Jade {

SoundCuePlayer::SoundCuePlayer() : _musicState(0) {
	_cueSheet = new Aurora::CueSheet(ResMan.getResource("cuesheet", Aurora::kFileTypeTXT));
}

SoundCuePlayer::~SoundCuePlayer() {
	delete _cueSheet;
}

void SoundCuePlayer::playSoundCue(const Common::UString &soundCueName) {
	Aurora::SoundBank &soundbank = fetchSoundBank(_cueSheet->getSoundBankName(soundCueName));

	playSoundCue(soundbank, soundCueName);
}

void SoundCuePlayer::playSoundCue(const Common::UString &soundCueName, const uint32 soundIndex) {
	Aurora::SoundBank &soundbank = fetchSoundBank(_cueSheet->getSoundBankName(soundCueName));
	const Aurora::SoundCue &soundCue = soundbank.fetchSoundCue(soundCueName);

	if (soundCue.soundList.size() <= soundIndex)
		throw Common::Exception("Sound cue only has %lu sound entries, index %i too large.",
		    soundCue.soundList.size(), soundIndex);

	playSound(soundbank.fetchSound(soundCue.soundList[soundIndex]));
}

void SoundCuePlayer::playSoundSetCue(const Common::UString &soundSet, const Common::UString &soundRef) {
	Aurora::SoundBank &soundbank = fetchSoundBank(soundSet);

	const Aurora::TwoDAFile &soundset = TwoDAReg.get2DA("soundset");
	const Aurora::TwoDARow &soundRefRow = soundset.getRow("label", soundRef);

	playSoundCue(soundbank, soundRefRow.getString("name"));
}

void SoundCuePlayer::changeMusicState(const uint32 musicState) {
	Aurora::SoundBank &soundbank = fetchSoundBank(_cueSheet->getSoundBankName("music"));
	const Aurora::SoundCue &soundCue = soundbank.fetchSoundCue("music");
	Aurora::Sound &sound = chooseSound(soundbank, soundCue, musicState);

	playMusic(sound);
}

uint32 SoundCuePlayer::getMusicState() const {
	return _musicState;
}

void SoundCuePlayer::playSoundCue(Aurora::SoundBank &soundbank, const Common::UString &soundCueName, uint32 variable) {
	const Aurora::SoundCue &soundCue = soundbank.fetchSoundCue(soundCueName);
	Aurora::Sound &sound = chooseSound(soundbank, soundCue, variable);

	playSound(sound);
}

void SoundCuePlayer::playSound(Aurora::Sound &sound) {
	const Common::UString &resref = chooseSoundResource(sound);

	if (SoundMan.isPlaying(_soundChannel))
		SoundMan.stopChannel(_soundChannel);
	_soundChannel = ::Engines::playSound(resref, Sound::kSoundTypeSFX, false);
}

void SoundCuePlayer::playMusic(Aurora::Sound &sound) {
	const Common::UString &resref = chooseSoundResource(sound);

	if (SoundMan.isPlaying(_musicChannel))
		SoundMan.stopChannel(_musicChannel);
	_musicChannel = ::Engines::playSound(resref, Sound::kSoundTypeMusic, false);
}

Aurora::Sound &SoundCuePlayer::chooseSound(Aurora::SoundBank &soundbank, const Aurora::SoundCue &soundCue,
                                           uint32 variable) {

	// TODO selection
	switch (soundCue.soundSelectionMode) {
	case 3: // music with music state
		for (uint32 i = 0; i < soundCue.soundList.size(); i++) {
			if (soundCue.soundList[i].max >= variable && soundCue.soundList[i].min <= variable)
				return soundbank.fetchSound(soundCue.soundList[i]);
		}
	default:
		return soundbank.fetchSound(soundCue.soundList.back());
	}
}

const Common::UString &SoundCuePlayer::chooseSoundResource(Aurora::Sound &sound) {
	uint32 wavebankIndex = 0;
	if (sound.wavebankEntries.size() > 1)
		wavebankIndex = rand() % sound.wavebankEntries.size();

	const Aurora::WaveBankEntry &wbEntry = sound.wavebankEntries[wavebankIndex];

	Aurora::WaveBank &wavebank = fetchWaveBank(wbEntry.wavebankName);
	return wavebank.getResref(wbEntry.wavebankIndex);
}

Aurora::WaveBank &SoundCuePlayer::fetchWaveBank(const Common::UString &wavebankName) {
	WaveBankMap::iterator it = _wavebankMap.find(wavebankName);
	if (it == _wavebankMap.end()) {

		Common::UString resRef;
		if (ResMan.hasResource(wavebankName + "_xwb", Aurora::kFileTypeTXT))
			resRef = wavebankName + "_xwb";
		else if (ResMan.hasResource("ss_j" + wavebankName + "_xwb", Aurora::kFileTypeTXT))
			// sounds sets
			resRef = "ss_j" + wavebankName + "_xwb";
		else if (ResMan.hasResource("j" + wavebankName + "_xwb", Aurora::kFileTypeTXT))
			// voice overs
			resRef = "j" + wavebankName + "_xwb";
		else
			throw Common::Exception("No resource for wavebank %s", wavebankName.c_str());

		Aurora::WaveBank wavebank(ResMan.getResource(resRef, Aurora::kFileTypeTXT));
		std::pair<Common::UString, Aurora::WaveBank> pair(wavebankName, wavebank);
		it = _wavebankMap.insert(pair).first;
	}
	return it->second;
}

Aurora::SoundBank &SoundCuePlayer::fetchSoundBank(const Common::UString &soundbankName) {
	SoundBankMap::iterator it = _soundbankMap.find(soundbankName);
	if (it == _soundbankMap.end()) {
		if (ResMan.hasResource(soundbankName + "_xsb", Aurora::kFileTypeTXT)) {
			Aurora::SoundBank soundbank(ResMan.getResource(soundbankName + "_xsb", Aurora::kFileTypeTXT));
			std::pair<Common::UString, Aurora::SoundBank> pair(soundbankName, soundbank);
			it = _soundbankMap.insert(pair).first;
		} else {
			throw Common::Exception("No resource for soundbank %s_xsb", soundbankName.c_str());
		}
	}
	return it->second;
}

} // End of namespace Jade

} // End of namespace Engines
