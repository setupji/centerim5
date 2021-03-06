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
/// TextView class.
///
/// @ingroup cppconsui

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "Widget.h"

#include <deque>

namespace CppConsUI {

/// Line oriented text view widget.
class TextView : public Widget {
public:
  TextView(int w, int h, bool autoscroll_ = false, bool scrollbar_ = false);
  virtual ~TextView() override;

  // Widget
  virtual int draw(Curses::ViewPort area, Error &error) override;

  /// Appends text after the last line.
  virtual void append(const char *text, int color = 0);

  /// Inserts text before specified line number. Text can contain multiple lines
  /// and should end with the '\n' character just before the '\0' byte.
  virtual void insert(std::size_t line_num, const char *text, int color = 0);

  /// Removes a specified line.
  virtual void erase(std::size_t line_num);

  /// Removes specified range of lines. Parameter end_line represents the line
  /// after the last removed line, thus range of <start_line, end_line) lines is
  /// removed.
  virtual void erase(std::size_t start_line, std::size_t end_line);

  /// Removes all lines.
  virtual void clear();

  /// Returns string for a specified line number.
  virtual const char *getLine(std::size_t line_num) const;

  /// Returns count of all lines.
  virtual std::size_t getLinesNumber() const;

  virtual void setAutoScroll(bool new_autoscroll);
  virtual bool hasAutoScroll() const { return autoscroll_; }

  virtual void setScrollBar(bool new_scrollbar);
  virtual bool hasScrollBar() const { return scrollbar_; }

protected:
  /// Struct Line saves a real line. All text added into TextView is split on
  /// '\\n' character and stored into Line objects.
  struct Line {
    /// UTF-8 encoded text. Note: Newline character is not part of text.
    char *text;

    /// Text length in characters.
    std::size_t length;

    /// Color number.
    int color;

    Line(const char *text_, std::size_t bytes, int color_);
    virtual ~Line();
  };

  /// ScreenLine represents an on-screen line.
  struct ScreenLine {
    /// Parent Line that this ScreenLine shows.
    Line *parent;

    /// Pointer into parent's text where this ScreenLine starts.
    const char *text;

    /// Length in characters (Unicode).
    int length;

    ScreenLine(Line &parent_, const char *text_, int length_);
  };

  typedef std::deque<Line *> Lines;
  typedef std::deque<ScreenLine> ScreenLines;

  std::size_t view_top_;
  bool autoscroll_;
  bool autoscroll_suspended_;
  bool scrollbar_;

  /// Array of real lines.
  Lines lines_;

  /// Array of on-screen lines.
  ScreenLines screen_lines_;

  // Widget
  virtual void updateArea() override;

  virtual const char *proceedLine(
    const char *text, int area_width, int *res_length) const;

  /// Recalculates on-screen lines for a specified line number.
  virtual std::size_t updateScreenLines(
    std::size_t line_num, std::size_t start = 0);

  /// Recalculates all screen lines.
  virtual void updateAllScreenLines();

  virtual std::size_t eraseScreenLines(std::size_t line_num,
    std::size_t start = 0, std::size_t *deleted = nullptr);

private:
  CONSUI_DISABLE_COPY(TextView);

  void actionScroll(int direction);

  void declareBindables();
};

} // namespace CppConsUI

#endif // TEXTVIEW_H

// vim: set tabstop=2 shiftwidth=2 textwidth=80 expandtab:
