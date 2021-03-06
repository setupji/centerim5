// Copyright (C) 2007 Mark Pustjens <pustjens@dds.nl>
// Copyright (C) 2010-2015 Petr Pavlu <setup@dagobah.cz>
//
// This file is part of CenterIM.
//
// CenterIM is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// CenterIM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CenterIM.  If not, see <http://www.gnu.org/licenses/>.

/// @file
/// Label class implementation.
///
/// @ingroup cppconsui

#include "Label.h"

#include "ColorScheme.h"

#include <cstring>

namespace CppConsUI {

Label::Label(int w, int h, const char *text) : Widget(w, h), text_(nullptr)
{
  setText(text);
}

Label::Label(const char *text) : Widget(AUTOSIZE, AUTOSIZE), text_(nullptr)
{
  setText(text);
}

Label::~Label()
{
  delete[] text_;
}

int Label::draw(Curses::ViewPort area, Error &error)
{
  int attrs;
  DRAW(getAttributes(ColorScheme::PROPERTY_LABEL_TEXT, &attrs, error));
  DRAW(area.attrOn(attrs, error));

  // Print text.
  int y = 0;
  const char *start, *end;
  start = end = text_;
  int printed;
  while (*end != '\0') {
    if (*end == '\n') {
      DRAW(area.addString(
        0, y, real_width_ * (real_height_ - y), start, end, error, &printed));
      y += (printed / real_width_) + 1;
      start = end + 1;
    }
    ++end;
  }
  DRAW(
    area.addString(0, y, real_width_ * (real_height_ - y), start, end, error));

  DRAW(area.attrOff(attrs, error));

  return 0;
}

void Label::setText(const char *new_text)
{
  std::size_t size = 1;
  if (new_text != nullptr)
    size += std::strlen(new_text);
  auto new_storage = new char[size];
  if (new_text != nullptr)
    std::strcpy(new_storage, new_text);
  else
    new_storage[0] = '\0';

  delete[] text_;
  text_ = new_storage;

  // Update wish height.
  int h = 1;
  for (const char *cur = text_; *cur; ++cur)
    if (*cur == '\n')
      ++h;
  setWishHeight(h);

  redraw();
}

} // namespace CppConsUI

// vim: set tabstop=2 shiftwidth=2 textwidth=80 expandtab:
