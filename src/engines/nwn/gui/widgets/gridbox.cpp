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

/** @file engines/nwn/gui/widgets/gridbox.cpp
 *  A NWN gridbox widget.
 */

#include "common/util.h"

#include "graphics/graphics.h"

#include "engines/aurora/util.h"

#include "engines/nwn/gui/widgets/scrollbar.h"
#include "engines/nwn/gui/widgets/gridbox.h"

namespace Engines {

namespace NWN {

WidgetGridBox::WidgetGridBox(::Engines::GUI &gui, const Common::UString &tag,
                             const Common::UString &model, float innerHSpace, float innerVSpace) :
                             WidgetListBox(gui, tag, model), _innerHSpace(innerHSpace),
                             _innerVSpace(innerVSpace) {
}

WidgetGridBox::~WidgetGridBox() {
}

void WidgetGridBox::subActive(Widget &widget) {
	if (widget.getTag().endsWith("#Up")) {
		scrollUp(1);
		return;
	}

	if (widget.getTag().endsWith("#Down")) {
		scrollDown(1);
		return;
	}

	if (widget.getTag().endsWith("#Bar")) {
		if (_items.size() - _visibleItems.size() == 0)
			return;

		uint maxIfFilled = _items.size() + _itemsByRow - _items.size() % _itemsByRow;
		uint maxVisible  = floor(_contentHeight / _items.front()->getHeight()) * _itemsByRow;

		uint startItem = floor(_scrollbar->getState() * (maxIfFilled - maxVisible)
		                       / _itemsByRow) * _itemsByRow;

		if (startItem == _startItem)
			return;

		_startItem = startItem;
		updateVisible();
		return;
	}

	WidgetListItem *listItem = dynamic_cast<WidgetListItem *>(&widget);
	if (listItem) {
		if (_selectedItem != listItem->getItemNumber()) {
			_selectedItem = listItem->getItemNumber();
			setActive(true);
			playSound("gui_button", Sound::kSoundTypeSFX);
		}
	}
}

void WidgetGridBox::mouseDown(uint8 UNUSED(state), float x, float y) {
	if (isDisabled())
		return;

	float wX, wY, wZ;
	getPosition(wX, wY, wZ);

	// Check if we clicked on the scrollbar area
	if (_scrollbar) {
		if (x > (wX + getWidth() - 20)) {
			uint scroll = _visibleItems.size() / _itemsByRow;
			if (y > _scrollbar->getBarPosition())
				scrollUp(scroll);
			else
				scrollDown(scroll);

			return;
		}
	}
}

void WidgetGridBox::unlock() {
	assert(_locked);
	_locked = false;

	if (_items.empty()) {
		GfxMan.unlockFrame();
		return;
	}

	_itemsByRow = MIN<uint>(_contentWidth / _items.front()->getWidth(), _items.size());

	uint vCount = MIN<uint>(_contentHeight / _items.front()->getHeight(), _items.size())
	              * _itemsByRow;

	if ((vCount == 0) || (vCount == _visibleItems.size())) {
		GfxMan.unlockFrame();
		return;
	}

	assert(_visibleItems.size() < vCount);
	_visibleItems.reserve(vCount);

	uint start = _startItem + _visibleItems.size();

	float itemHeight = _items.front()->getHeight();
	float itemWidth  = _items.front()->getWidth();
	uint  row        = 0;
	uint  column     = 0;

	// If we reach the last row, compute the items that remains.
	if (_items.size() - _startItem <= _visibleItems.size())
		vCount -= _items.size() % _itemsByRow;

	while (_visibleItems.size() < vCount) {
		WidgetListItem *item = _items[start++];

		// WORKAROUND: The x axis is shifted by 2 pixels in order to correctly render in
		// the charportrait widget.
		float itemY = _contentY - (row + 1) * (itemHeight + _innerVSpace) + _innerVSpace;
		float itemX = _contentX + (column * (itemWidth + _innerHSpace)) - 2.0;
		item->setPosition(itemX, itemY, _contentZ - 5.0);
		_visibleItems.push_back(item);

		if (isVisible())
			_visibleItems.back()->show();

		++column;

		if (column == _itemsByRow) {
			++row;
			column = 0;
		}
	}

	updateScrollbarLength();
	updateScrollbarPosition();

	GfxMan.unlockFrame();
}

void WidgetGridBox::updateScrollbarLength() {
	if (!_scrollbar)
		return;

	if (_visibleItems.empty())
		_scrollbar->setLength(1.0);
	else {
		_scrollbar->setLength(((float) _visibleItems.size()) / (_items.size()
		                                                        + (_items.size() % _itemsByRow)));
	}
}

void WidgetGridBox::updateVisible() {
	if (_visibleItems.empty())
		return;

	GfxMan.lockFrame();

	for (uint i = 0; i < _visibleItems.size(); i++)
		_visibleItems[i]->hide();

	if (_visibleItems.size() > _items.size())
		_visibleItems.resize(_items.size());

	float itemHeight = _items.front()->getHeight() + _innerVSpace;
	float itemWidth  = _items.front()->getWidth() + _innerHSpace;
	float itemY      = _contentY - itemHeight + _innerVSpace;

	uint column = 0;
	int  count  = 0;

	if (_items.size() - _startItem <= _visibleItems.size())
		count = _items.size() % _itemsByRow;

	for (uint i = 0; i < (_visibleItems.size() - count); i++) {
		WidgetListItem *item = _items[_startItem + i];

		if (column == _itemsByRow) {
			itemY -= itemHeight;
			column = 0;
		}

		// WORKAROUND: The x axis is shifted by 2 pixels in order to correctly render in
		// the charportrait widget.
		item->setPosition(_contentX + column * itemWidth - 2.0, itemY, _contentZ - 5.0);
		_visibleItems[i] = item;

		if (isVisible())
			_visibleItems[i]->show();

		++column;
	}

	GfxMan.unlockFrame();
}

void WidgetGridBox::scrollUp(uint n) {
	if (_visibleItems.empty())
		return;

	if (_startItem == 0)
		return;

	_startItem -= MIN<uint>(n * _itemsByRow, _startItem);

	updateVisible();
	updateScrollbarPosition();
}

void WidgetGridBox::scrollDown(uint n) {
	if (_visibleItems.empty())
		return;

	if (_startItem + _visibleItems.size() >= _items.size())
		return;


	_startItem += MIN<uint>(n * _itemsByRow, (_items.size() + _items.size() %
	                                          _itemsByRow) - _visibleItems.size() - _startItem);

	updateVisible();
	updateScrollbarPosition();
}

} // End of namespace NWN

} // End of namespace Engines
