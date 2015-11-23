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

#ifndef AURORA_WAVEBANK_H
#define AURORA_WAVEBANK_H

#include <vector>

#include "src/common/strutil.h"
#include "src/common/ustring.h"

namespace Common {
	class SeekableReadStream;
}

namespace Aurora {

/** 
 * A wave bank is basically a list of sound files with format data.
 * 
 * The data in the wave bank files refers to WAV or WMA files while the
 * sound files are actually in OGG format.
 * 
 * This suggests that the format data is not used; only the resrefs and
 * their position in the list is relevant.
 */
class WaveBank {
public:
	WaveBank(Common::SeekableReadStream *xwb);
	~WaveBank();

	Common::UString &getResref(const uint32 pos);

private:
	Common::UString _name;
	std::vector<Common::UString> _entries;

	/** Load a XWB file.
	 *
	 *  @param xwb A stream of an XWB file.
	 */
	void load(Common::SeekableReadStream &xwb);
};

} // End of namespace Aurora

#endif // AURORA_WAVEBANK_H
