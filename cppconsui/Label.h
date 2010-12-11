/*
 * Copyright (C) 2007 by Mark Pustjens <pustjens@dds.nl>
 * Copyright (C) 2010 by CenterIM developers
 *
 * This file is part of CenterIM.
 *
 * CenterIM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CenterIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

/**
 * @file
 * Label class.
 *
 * @ingroup cppconsui
 */

#ifndef __LABEL_H__
#define __LABEL_H__

#include "Widget.h"

/**
 * A widget that displays a small to medium amount of text.
 */
class Label
: public Widget
{
public:
  Label(int w, int h, const gchar *text_ = NULL);
  explicit Label(const gchar *text_ = NULL);
  virtual ~Label();

  // Widget
  virtual void Draw();

  /**
   * Sets a new text and redraws itself.
   */
  virtual void SetText(const gchar *new_text);
  /**
   * Returns previously set text.
   */
  virtual const gchar *GetText() const { return text; }

protected:
  gchar *text;

private:
  Label(const Label&);
  Label& operator=(const Label&);
};

#endif // __LABEL_H__
