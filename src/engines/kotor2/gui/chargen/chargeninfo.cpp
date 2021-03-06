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
 *  The KotOR 2 character generation info.
 */

#include "src/common/strutil.h"
#include "src/common/error.h"
#include "src/common/scopedptr.h"

#include "chargeninfo.h"

namespace Engines {

namespace KotOR2 {

CharacterGenerationInfo *CharacterGenerationInfo::createRandomMaleConsular() {
	CharacterGenerationInfo *info = new CharacterGenerationInfo();
	info->_gender = kGenderMale;
	info->_class = kClassJediConsular;
	info->_skin = Skin(std::rand() % kSkinMAX);
	switch (info->_skin) {
		case kSkinH:
			info->_face = std::rand() % 2;
			break;
		default:
			info->_face = std::rand() % 5;
			break;
	}
	return info;
}

CharacterGenerationInfo *CharacterGenerationInfo::createRandomFemaleConsular() {
	CharacterGenerationInfo *info = new CharacterGenerationInfo();
	info->_gender = kGenderFemale;
	info->_class = kClassJediConsular;
	info->_skin = Skin(std::rand() % kSkinMAX);
	switch (info->_skin) {
		case kSkinH:
			info->_face = std::rand() % 2;
			break;
		default:
			info->_face = std::rand() % 5;
			break;
	}
	return info;
}

CharacterGenerationInfo *CharacterGenerationInfo::createRandomMaleGuardian() {
	CharacterGenerationInfo *info = new CharacterGenerationInfo();
	info->_gender = kGenderMale;
	info->_class = kClassJediGuardian;
	info->_skin = Skin(std::rand() % kSkinMAX);
	switch (info->_skin) {
		case kSkinH:
			info->_face = std::rand() % 2;
			break;
		default:
			info->_face = std::rand() % 5;
			break;
	}
	return info;
}

CharacterGenerationInfo *CharacterGenerationInfo::createRandomFemaleGuardian() {
	CharacterGenerationInfo *info = new CharacterGenerationInfo();
	info->_gender = kGenderFemale;
	info->_class = kClassJediGuardian;
	info->_skin = Skin(std::rand() % kSkinMAX);
	switch (info->_skin) {
		case kSkinH:
			info->_face = std::rand() % 2;
			break;
		default:
			info->_face = std::rand() % 5;
			break;
	}
	return info;
}

CharacterGenerationInfo *CharacterGenerationInfo::createRandomMaleSentinel() {
	CharacterGenerationInfo *info = new CharacterGenerationInfo();
	info->_gender = kGenderMale;
	info->_class = kClassJediSentinel;
	info->_skin = Skin(std::rand() % kSkinMAX);
	switch (info->_skin) {
		case kSkinH:
			info->_face = std::rand() % 2;
			break;
		default:
			info->_face = std::rand() % 5;
			break;
	}
	return info;
}

CharacterGenerationInfo *CharacterGenerationInfo::createRandomFemaleSentinel() {
	CharacterGenerationInfo *info = new CharacterGenerationInfo();
	info->_gender = kGenderFemale;
	info->_class = kClassJediSentinel;
	info->_skin = Skin(std::rand() % kSkinMAX);
	switch (info->_skin) {
		case kSkinH:
			info->_face = std::rand() % 2;
			break;
		default:
			info->_face = std::rand() % 5;
			break;
	}
	return info;
}

CharacterGenerationInfo::CharacterGenerationInfo() {
}

const Common::UString &CharacterGenerationInfo::getName() const {
	return _name;
}

Skin CharacterGenerationInfo::getSkin() const {
	return _skin;
}

unsigned int CharacterGenerationInfo::getFace() const {
	return _face;
}

Gender CharacterGenerationInfo::getGender() const {
	return _gender;
}

Class CharacterGenerationInfo::getClass() const {
	return _class;
}

void CharacterGenerationInfo::setName(const Common::UString &name) {
	_name = name;
}

void CharacterGenerationInfo::setSkin(Skin skin) {
	_skin = skin;
}

void CharacterGenerationInfo::setFace(unsigned int face) {
	_face = face;
}

Common::UString CharacterGenerationInfo::getBodyId() const {
	Common::UString body;

	body += "p";

	switch (_gender) {
		case kGenderMale:
			body += "m";
			break;
		case kGenderFemale:
			body += "f";
			break;
		default:
			throw Common::Exception("Invalid gender");
	}

	body += "bam";

	return body;
}

Common::UString CharacterGenerationInfo::getBodyTextureId() const {
	Common::UString body;

	body += "p";

	switch (_gender) {
		case kGenderMale:
			body += "m";
			break;
		case kGenderFemale:
			body += "f";
			break;
		default:
			throw Common::Exception("Invalid gender");
	}

	body += "bam";

	switch (_skin) {
		case kSkinA:
		case kSkinH:
			body += "a";
			break;
		case kSkinB:
			body += "b";
			break;
		case kSkinC:
			body += "c";
			break;
		default:
			throw Common::Exception("Invalid skin");
	}

	body += "01";

	return body;
}

Common::UString CharacterGenerationInfo::getHeadId() const {
	Common::UString head;

	head += "p";

	switch (_gender) {
		case kGenderMale:
			head += "m";
			break;
		case kGenderFemale:
			head += "f";
			break;
		default:
			throw Common::Exception("Invalid gender");
	}

	head += "h";

	switch (_skin) {
		case kSkinA:
			head += "a";
			break;
		case kSkinB:
			head += "b";
			break;
		case kSkinC:
			head += "c";
			break;
		case kSkinH:
			head += "h";
			break;
		default:
			throw Common::Exception("Invalid skin");
	}

	if (_face >= 10)
		head += Common::composeString(getFaceId());
	else
		head += "0" + Common::composeString(getFaceId());

	return head;
}

Common::UString CharacterGenerationInfo::getPortrait() const {
	Common::UString portrait;
	portrait += "po_p";

	switch (_gender) {
		case kGenderMale:
			portrait += "m";
			break;
		case kGenderFemale:
			portrait += "f";
			break;
		default:
			throw Common::Exception("Gender unknown for creating portrait string");
	}

	portrait += "h";

	switch (_skin) {
		case kSkinA:
			switch (_face) {
				case 0:
					portrait += "a01";
					break;
				case 1:
					portrait += "a03";
					break;
				case 2:
					if (_gender == kGenderFemale)
						portrait += "a04";
					else if (_gender == kGenderMale)
						portrait += "a05";
					break;
				case 3:
					if (_gender == kGenderFemale)
						portrait += "a05";
					else if (_gender == kGenderMale)
						portrait += "a06";
					break;
				case 4:
					if (_gender == kGenderFemale)
						portrait += "a06";
					else if (_gender == kGenderMale)
						portrait += "a07";
					break;
				default:
					throw Common::Exception("invalid face id");
			}
			break;
		case kSkinB:
			portrait += "b";
			if (_gender == kGenderFemale)
				portrait += ("0" + Common::composeString(_face + 1));
			else
				if (_face + 6 >= 10)
					portrait += Common::composeString(_face + 6);
				else
					portrait += ("0" + Common::composeString(_face + 6));
			break;
		case kSkinC:
			switch (_face) {
				case 0:
					portrait += "c01";
					break;
				case 1:
					if (_gender == kGenderFemale)
						portrait += "c02";
					else if (_gender == kGenderMale)
						portrait += "c03";
					break;
				case 2:
					if (_gender == kGenderFemale)
						portrait += "c05";
					else if (_gender == kGenderMale)
						portrait += "c04";
					break;
				case 3:
					portrait += "c06";
					break;
				case 4:
					portrait += "c07";
					break;
				default:
					throw Common::Exception("invalid face id");
			}
			break;
		case kSkinH:
			portrait += ("h0" + Common::composeString(_face + 1));
			break;
		default:
			throw Common::Exception("invalid skin id");
	}

	return portrait;
}

Creature *CharacterGenerationInfo::getCharacter() const {
	Common::ScopedPtr<Creature> creature(new Creature());

	creature->createPC(*this);
	return creature.release();
}

unsigned int CharacterGenerationInfo::getFaceId() const {
	switch (_skin) {
		case kSkinA:
			switch (_face) {
				case 0: return 1;
				case 1: return 3;
				case 2:
					if (_gender == kGenderFemale) return 4;
					else if (_gender == kGenderMale) return 5;
					else throw Common::Exception("invalid gender");
				case 3:
					if (_gender == kGenderFemale) return 5;
					else if (_gender == kGenderMale) return 6;
					else throw Common::Exception("invalid gender");
				case 4:
					if (_gender == kGenderFemale) return 6;
					else if (_gender == kGenderMale) return 7;
					else throw Common::Exception("invalid gender");
				default:
					throw Common::Exception("invalid face id");
			}
		case kSkinB:
			if (_gender == kGenderFemale) return _face + 1;
			else return _face + 6;
		case kSkinC:
			switch (_face) {
				case 0: return 1;
				case 1:
					if (_gender == kGenderFemale) return 2;
					else if (_gender == kGenderMale) return 3;
					else throw Common::Exception("invalid gender");
				case 2:
					if (_gender == kGenderFemale) return 5;
					else if (_gender == kGenderMale) return 4;
					else throw Common::Exception("invalid gender");
				case 3: return 6;
				case 4: return 7;
				default:
					throw Common::Exception("invalid face id");
			}
		case kSkinH: return _face + 1;
		default:
			throw Common::Exception("invalid skin id");
	}
}

} // End of namespace KotOR2

} // End of namespace Engines
