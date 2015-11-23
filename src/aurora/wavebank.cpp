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
 *  Handling Jade Empire's xwb files.
 */

#include "src/common/readstream.h"
#include "src/common/streamtokenizer.h"
#include "src/common/ustring.h"

#include "src/aurora/wavebank.h"

namespace Aurora {

WaveBank::WaveBank(Common::SeekableReadStream *xwb) {

	assert(xwb);

	try {
		load(*xwb);
	} catch (...) {
		delete xwb;
		throw;
	}

	delete xwb;
}

WaveBank::~WaveBank() {
}

Common::UString &WaveBank::getResref(const uint32 pos) {
	return _entries[pos];
}

void WaveBank::load(Common::SeekableReadStream &xwb) {
	Common::StreamTokenizer tokenizer(Common::StreamTokenizer::kRuleIgnoreAll);
	tokenizer.addSeparator(' ');
	tokenizer.addChunkEnd('\n');
	tokenizer.addIgnore('\r');

	_name = tokenizer.getToken(xwb);
	tokenizer.nextChunk(xwb);

	tokenizer.nextChunk(xwb); // type streaming or memory

	int32 numberEntries = 0;
	Common::parseString(tokenizer.getToken(xwb), numberEntries, false);
	tokenizer.nextChunk(xwb);

	std::vector<Common::UString> strings;
	for (int i = 0; i < numberEntries; i++) {
		tokenizer.getTokens(xwb, strings);

		// sound file resref
		_entries.push_back(strings[1].substr(strings[1].begin(), strings[1].findLast('.')));

		tokenizer.nextChunk(xwb);
	}
}

} // End of namespace Aurora
