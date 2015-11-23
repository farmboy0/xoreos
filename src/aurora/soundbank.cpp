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

#include "src/common/error.h"
#include "src/common/readstream.h"
#include "src/common/streamtokenizer.h"
#include "src/common/strutil.h"
#include "src/common/ustring.h"

#include "src/aurora/soundbank.h"

namespace Aurora {

SoundBank::SoundBank(Common::SeekableReadStream *xsb) {

	assert(xsb);

	try {
		load(*xsb);
	} catch (...) {
		delete xsb;
		throw;
	}

	delete xsb;
}

SoundBank::~SoundBank() {
}


Sound &SoundBank::fetchSound(const SoundEntry &soundEntry) {
	if (_soundList.size() <= soundEntry.index)
		throw Common::Exception("No sound %s found in sound bank %s", soundEntry.name.c_str(), _name.c_str());
	return _soundList[soundEntry.index];
}

SoundCue &SoundBank::fetchSoundCue(const Common::UString &cueName) {
	if (_soundCues.find(cueName) == _soundCues.end())
		throw Common::Exception("No sound cue %s found in sound bank %s", cueName.c_str(), _name.c_str());
	return _soundCues[cueName];
}

SoundCue &SoundBank::fetchSoundCue(const uint32 UNUSED(cueIndex)) {
	return _soundCues[""];
}

void SoundBank::load(Common::SeekableReadStream &xsb) {
	Common::StreamTokenizer tokenizer(Common::StreamTokenizer::kRuleIgnoreAll);
	tokenizer.addSeparator(' ');
	tokenizer.addChunkEnd('\n');
	tokenizer.addIgnore('\r');
	tokenizer.addQuote('"');

	_name = tokenizer.getToken(xsb);
	tokenizer.nextChunk(xsb);

	uint32 numberCategories = parseNumber(tokenizer.getToken(xsb));
	tokenizer.nextChunk(xsb);

	for (uint32 i = 0; i < numberCategories; i++) {
		// categories are ignored for now
		tokenizer.nextChunk(xsb);
	}

	uint32 numberSounds = parseNumber(tokenizer.getToken(xsb));
	tokenizer.nextChunk(xsb);

	for (uint32 i = 0; i < numberSounds; i++) {
		Sound sound;

		sound.name = tokenizer.getToken(xsb);
		// TODO token after sound name
		tokenizer.nextChunk(xsb);

		std::vector<Common::UString> tokenList;
		tokenizer.getTokens(xsb, tokenList);
		while(tokenList.size() != 1) {
			// TODO play properties
			tokenizer.nextChunk(xsb);
			tokenizer.getTokens(xsb, tokenList);
		}

		uint32 numberWaveBankEntries = parseNumber(tokenList[0]);
		tokenizer.nextChunk(xsb);

		for (uint32 j = 0; j < numberWaveBankEntries; j++) {
			WaveBankEntry entry;

			entry.wavebankName = tokenizer.getToken(xsb);
			entry.name = tokenizer.getToken(xsb);
			entry.wavebankIndex = parseNumber(tokenizer.getToken(xsb));
			// TODO last token always 0?

			sound.wavebankEntries.push_back(entry);

			tokenizer.nextChunk(xsb);
		}

		_soundList.push_back(sound);
	}

	uint32 numberSoundCues = parseNumber(tokenizer.getToken(xsb));
	tokenizer.nextChunk(xsb);

	for (uint32 i = 0; i < numberSoundCues; i++) {
		SoundCue cue;

		cue.name = tokenizer.getToken(xsb);
		Common::parseString(tokenizer.getToken(xsb), cue.soundSelectionMode);
		Common::parseString(tokenizer.getToken(xsb), cue.hasTransitionProperties);
		// TODO next token all 0?
		tokenizer.nextChunk(xsb);

		uint32 numberSoundsInCue = parseNumber(tokenizer.getToken(xsb));
		tokenizer.nextChunk(xsb);

		for (uint32 j = 0; j < numberSoundsInCue; j++) {
			SoundEntry entry;

			entry.name = tokenizer.getToken(xsb);
			entry.index = parseNumber(tokenizer.getToken(xsb));
			entry.max = parseNumber(tokenizer.getToken(xsb));
			entry.min = parseNumber(tokenizer.getToken(xsb));
			tokenizer.nextChunk(xsb);

			if (!entry.name.equals(_soundList[entry.index].name)) {
				warning("sound bank: %s, sound cue: %s, sound name: %s, sound index: %i doesnt match %s",
					_name.c_str(), cue.name.c_str(), entry.name.c_str(), entry.index, _soundList[entry.index].name.c_str());
			}

			cue.soundList.push_back(entry);
		}

		if (cue.hasTransitionProperties) {
			uint32 numberTransitionProperties = parseNumber(tokenizer.getToken(xsb));
			tokenizer.nextChunk(xsb);

			for (uint32 j = 0; j < numberTransitionProperties; j++) {
				// TODO transition properties
				tokenizer.nextChunk(xsb);
			}
		}

		_soundCues[cue.name] = cue;
	}
}

uint32 SoundBank::parseNumber(const Common::UString &numberString) const {
	Common::UString::iterator itStart = numberString.begin();
	for (; itStart != numberString.end(); ++itStart)
		if (*itStart != '0')
			break;

	Common::UString s = numberString.substr(itStart, numberString.end());

	uint32 number = 0;
	if (!s.empty())
		Common::parseString(s, number, false);
	return number;
}

CueSheet::CueSheet(Common::SeekableReadStream *sheet) {

	assert(sheet);

	try {
		load(*sheet);
	} catch (...) {
		delete sheet;
		throw;
	}

	delete sheet;
}

CueSheet::~CueSheet() {
}

void CueSheet::getSoundBankNames(std::vector<Common::UString> &names) const {
	for (std::vector<Common::UString>::const_iterator it = _soundBankNames.begin(); it != _soundBankNames.end(); ++it) {
		names.push_back(*it);
	}
}

void CueSheet::getSoundCueNames(std::vector<Common::UString> &names, const Common::UString &xsbName) const {
	for (std::map<Common::UString, SoundBankEntry>::const_iterator it = _soundCues.begin(); it != _soundCues.end(); ++it) {
		if (xsbName.empty() || (*it).second.soundbankName.equalsIgnoreCase(xsbName))
			names.push_back((*it).first);
	}
}

Common::UString &CueSheet::getSoundBankName(const Common::UString &cueName) {
	SoundBankEntryMap::iterator it = _soundCues.find(cueName);
	if (it == _soundCues.end()) {
		throw Common::Exception("Sound cue %s not found in sound sheet", cueName.c_str());
	}
	return it->second.soundbankName;
}

void CueSheet::load(Common::SeekableReadStream &sheet) {
	Common::StreamTokenizer tokenizer(Common::StreamTokenizer::kRuleIgnoreAll);
	tokenizer.addSeparator(',');
	tokenizer.addChunkEnd('\n');
	tokenizer.addIgnore('\r');

	int32 numberSoundBanks = 0;
	Common::parseString(tokenizer.getToken(sheet), numberSoundBanks, false);
	tokenizer.nextChunk(sheet);

	for (int i = 0; i < numberSoundBanks; i++) {
		Common::UString soundBankName = tokenizer.getToken(sheet);
		_soundBankNames.push_back(soundBankName);
		tokenizer.nextChunk(sheet);
	}

	int32 numberSoundCues = 0;
	Common::parseString(tokenizer.getToken(sheet), numberSoundCues, false);
	tokenizer.nextChunk(sheet);

	uint32 soundBankIndex, soundCueIndex;
	for (int i = 0; i < numberSoundCues; i++) {
		SoundBankEntry entry;

		Common::parseString(tokenizer.getToken(sheet), soundBankIndex, false);
		entry.soundbankName = _soundBankNames[soundBankIndex];
		entry.name = tokenizer.getToken(sheet);
		Common::parseString(tokenizer.getToken(sheet), soundCueIndex, false);
		entry.soundCueIndex = soundCueIndex;

		_soundCues[entry.name] = entry;

		tokenizer.nextChunk(sheet);
	}
}

} // End of namespace Aurora
