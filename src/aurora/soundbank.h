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
 *  Handling Jade Empire's xsb files.
 */

#ifndef AURORA_SOUNDBANK_H
#define AURORA_SOUNDBANK_H

#include <vector>

#include "src/common/ustring.h"

namespace Common {
	class SeekableReadStream;
}

namespace Aurora {

/*
 * A wavebank entry points to an entry in a wavebank
 * and with it to a sound resource.
 */
struct WaveBankEntry {
	Common::UString wavebankName;
	Common::UString name;
	uint32 wavebankIndex;
};

/*
 * A sound describes the playing properties and a list
 * of sound resources that are used when playing the sound.
 */
struct Sound {
	Common::UString name;
	// TODO play properties
	std::vector<WaveBankEntry> wavebankEntries;
};

struct SoundEntry {
	Common::UString name;
	uint32 index;
	uint32 max;
	uint32 min;
};

/** 
 * A sound cue groups a list of different sounds.
 * It also describes how to choose which one should be played.
 */
struct SoundCue {
	Common::UString name;
	uint32 soundSelectionMode;
	bool hasTransitionProperties;
	std::vector<SoundEntry> soundList;
};

/** 
 * A sound bank contains a list of sound cues and a list of sound..
 * 
 * Each sound bank belongs to a list of categories
 * like 'Effects' that define the type of sounds it contains.
 */
class SoundBank {
public:
	SoundBank(Common::SeekableReadStream *xsb);
	~SoundBank();

	Sound &fetchSound(const SoundEntry &soundEntry);

	SoundCue &fetchSoundCue(const Common::UString &cueName);
	SoundCue &fetchSoundCue(const uint32 cueIndex);
private:
	Common::UString _name;
	std::map<Common::UString, SoundCue> _soundCues;
	std::vector<Sound> _soundList;

	/** Load a XSB file.
	 *
	 *  @param xsb A stream of an XSB file.
	 */
	void load(Common::SeekableReadStream &xsb);

	uint32 parseNumber(const Common::UString &numberString) const;
};

/*
 * The cue sheet lists a set of common sound banks and has a reference
 * of the sound cues contained in them.
 */
class CueSheet {
public:
	CueSheet(Common::SeekableReadStream *sheet);
	~CueSheet();

	void getSoundBankNames(std::vector<Common::UString> &names) const;
	void getSoundCueNames(std::vector<Common::UString> &names, const Common::UString &xsbName = "") const;
	Common::UString &getSoundBankName(const Common::UString &cueName);

private:
	struct SoundBankEntry {
		Common::UString soundbankName;
		Common::UString name;
		uint32 soundCueIndex;
	};

	typedef std::map<Common::UString, SoundBankEntry> SoundBankEntryMap;

	/** Map of sound cue name to sound cue entry. */
	SoundBankEntryMap _soundCues;
	/** List of all sound bank names in the cue sheet. */
	std::vector<Common::UString> _soundBankNames;

	/** Load a cue sheet file.
	 *
	 *  @param sheet A stream of an cue sheet file.
	 */
	void load(Common::SeekableReadStream &sheet);
};

} // End of namespace Aurora

#endif // AURORA_SOUNDBANK_H
