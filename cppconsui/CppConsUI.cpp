// Copyright (C) 2013-2015 Petr Pavlu <setup@dagobah.cz>
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

#include "CppConsUI.h"

#include "ColorScheme.h"
#include "CoreManager.h"
#include "KeyConfig.h"

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace CppConsUI {

ColorScheme *color_scheme = nullptr;
CoreManager *core_manager = nullptr;
KeyConfig *key_config = nullptr;

Error::Error(ErrorCode code, const char *string)
  : error_code_(code), error_string_(nullptr)
{
  setString(string);
}

Error::Error(const Error &other)
{
  assert(other.error_string_ != nullptr);

  error_code_ = other.error_code_;

  std::size_t size = std::strlen(other.error_string_) + 1;
  error_string_ = new char[size];
  std::strcpy(error_string_, other.error_string_);
}

Error &Error::operator=(const Error &other)
{
  assert(other.error_string_ != nullptr);

  std::size_t size = std::strlen(other.error_string_) + 1;
  auto new_string = new char[size];
  std::strcpy(new_string, other.error_string_);

  error_code_ = other.error_code_;
  delete[] error_string_;
  error_string_ = new_string;

  return *this;
}

Error::~Error()
{
  delete[] error_string_;
}

void Error::setCode(ErrorCode code)
{
  error_code_ = code;
}

void Error::setString(const char *string)
{
  std::size_t size = 1;
  if (string != nullptr)
    size += std::strlen(string);
  auto new_string = new char[size];
  if (string != nullptr)
    std::strcpy(new_string, string);
  else
    new_string[0] = '\0';

  delete[] error_string_;
  error_string_ = new_string;
}

void Error::setFormattedString(const char *format, ...)
{
  assert(format != nullptr);

  va_list args;

  va_start(args, format);
  int size = std::vsnprintf(nullptr, 0, format, args) + 1;
  va_end(args);

  auto new_string = new char[size];

  va_start(args, format);
  std::vsprintf(new_string, format, args);
  va_end(args);

  delete[] error_string_;
  error_string_ = new_string;
}

void Error::clear()
{
  error_code_ = ERROR_NONE;
  delete[] error_string_;
  error_string_ = nullptr;
}

void initializeConsUI(AppInterface &interface)
{
  assert(color_scheme == nullptr);
  assert(core_manager == nullptr);
  assert(key_config == nullptr);

  // Initialize ColorScheme and KeyConfig. These cannot fail.
  color_scheme = new ColorScheme;
  key_config = new KeyConfig;

  // CoreManager depends on KeyConfig so it has to be initialized after it.
  core_manager = new CoreManager(interface);
}

void finalizeConsUI()
{
  assert(color_scheme != nullptr);
  assert(core_manager != nullptr);
  assert(key_config != nullptr);

  // Destroy CoreManager, KeyConfig and ColorScheme.
  delete core_manager;
  core_manager = nullptr;
  delete key_config;
  key_config = nullptr;
  delete color_scheme;
  color_scheme = nullptr;
}

ColorScheme *getColorSchemeInstance()
{
  assert(color_scheme != nullptr);
  return color_scheme;
}

CoreManager *getCoreManagerInstance()
{
  assert(core_manager != nullptr);
  return core_manager;
}

KeyConfig *getKeyConfigInstance()
{
  assert(key_config != nullptr);
  return key_config;
}

namespace UTF8 {

// Some code below is based on the GLib code.

// Bits  Length  Byte 1    Byte 2    Byte 3    Byte 4    Byte 5    Byte 6
//   7     1     0xxxxxxx
//  11     2     110xxxxx  10xxxxxx
//  16     3     1110xxxx  10xxxxxx  10xxxxxx
//  21     4     11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
//  26     5     111110xx  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx
//  31     6     1111110x  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx  10xxxxxx
UniChar getUniChar(const char *p)
{
  assert(p != nullptr);

  UniChar res;
  unsigned char c = *p++;
  int rest;

  if ((c & 0x80) == 0x00)
    return c & 0x7f;
  else if ((c & 0xe0) == 0xc0) {
    rest = 1;
    res = c & 0x1f;
  }
  else if ((c & 0xf0) == 0xe0) {
    rest = 2;
    res = c & 0x0f;
  }
  else if ((c & 0xf8) == 0xf0) {
    rest = 3;
    res = c & 0x07;
  }
  else if ((c & 0xfc) == 0xf8) {
    rest = 4;
    res = c & 0x03;
  }
  else if ((c & 0xfe) == 0xfc) {
    rest = 5;
    res = c & 0x01;
  }
  else
    return -1;

  while (rest-- > 0) {
    c = *p++;
    if ((c & 0xc0) != 0x80)
      return -1;
    res <<= 6;
    res |= c & 0x3f;
  }

  return res;
}

namespace {

struct Interval {
  UniChar start, end;
};

int interval_compare(const void *key, const void *elt)
{
  const UniChar uc = *static_cast<const UniChar *>(key);
  const Interval *interval = static_cast<const Interval *>(elt);

  if (uc < interval->start)
    return -1;
  if (uc > interval->end)
    return +1;

  return 0;
}

} // anonymous namespace

bool isUniCharWide(UniChar uc)
{
  static const Interval wide[] = {
    {0x1100, 0x115F},
    {0x231A, 0x231B},
    {0x2329, 0x232A},
    {0x23E9, 0x23EC},
    {0x23F0, 0x23F0},
    {0x23F3, 0x23F3},
    {0x25FD, 0x25FE},
    {0x2614, 0x2615},
    {0x2648, 0x2653},
    {0x267F, 0x267F},
    {0x2693, 0x2693},
    {0x26A1, 0x26A1},
    {0x26AA, 0x26AB},
    {0x26BD, 0x26BE},
    {0x26C4, 0x26C5},
    {0x26CE, 0x26CE},
    {0x26D4, 0x26D4},
    {0x26EA, 0x26EA},
    {0x26F2, 0x26F3},
    {0x26F5, 0x26F5},
    {0x26FA, 0x26FA},
    {0x26FD, 0x26FD},
    {0x2705, 0x2705},
    {0x270A, 0x270B},
    {0x2728, 0x2728},
    {0x274C, 0x274C},
    {0x274E, 0x274E},
    {0x2753, 0x2755},
    {0x2757, 0x2757},
    {0x2795, 0x2797},
    {0x27B0, 0x27B0},
    {0x27BF, 0x27BF},
    {0x2B1B, 0x2B1C},
    {0x2B50, 0x2B50},
    {0x2B55, 0x2B55},
    {0x2E80, 0x2E99},
    {0x2E9B, 0x2EF3},
    {0x2F00, 0x2FD5},
    {0x2FF0, 0x2FFB},
    {0x3000, 0x303E},
    {0x3041, 0x3096},
    {0x3099, 0x30FF},
    {0x3105, 0x312F},
    {0x3131, 0x318E},
    {0x3190, 0x31BA},
    {0x31C0, 0x31E3},
    {0x31F0, 0x321E},
    {0x3220, 0x3247},
    {0x3250, 0x4DBF},
    {0x4E00, 0xA48C},
    {0xA490, 0xA4C6},
    {0xA960, 0xA97C},
    {0xAC00, 0xD7A3},
    {0xF900, 0xFAFF},
    {0xFE10, 0xFE19},
    {0xFE30, 0xFE52},
    {0xFE54, 0xFE66},
    {0xFE68, 0xFE6B},
    {0xFF01, 0xFF60},
    {0xFFE0, 0xFFE6},
    {0x16FE0, 0x16FE3},
    {0x17000, 0x187F7},
    {0x18800, 0x18AF2},
    {0x1B000, 0x1B11E},
    {0x1B150, 0x1B152},
    {0x1B164, 0x1B167},
    {0x1B170, 0x1B2FB},
    {0x1F004, 0x1F004},
    {0x1F0CF, 0x1F0CF},
    {0x1F18E, 0x1F18E},
    {0x1F191, 0x1F19A},
    {0x1F200, 0x1F202},
    {0x1F210, 0x1F23B},
    {0x1F240, 0x1F248},
    {0x1F250, 0x1F251},
    {0x1F260, 0x1F265},
    {0x1F300, 0x1F320},
    {0x1F32D, 0x1F335},
    {0x1F337, 0x1F37C},
    {0x1F37E, 0x1F393},
    {0x1F3A0, 0x1F3CA},
    {0x1F3CF, 0x1F3D3},
    {0x1F3E0, 0x1F3F0},
    {0x1F3F4, 0x1F3F4},
    {0x1F3F8, 0x1F43E},
    {0x1F440, 0x1F440},
    {0x1F442, 0x1F4FC},
    {0x1F4FF, 0x1F53D},
    {0x1F54B, 0x1F54E},
    {0x1F550, 0x1F567},
    {0x1F57A, 0x1F57A},
    {0x1F595, 0x1F596},
    {0x1F5A4, 0x1F5A4},
    {0x1F5FB, 0x1F64F},
    {0x1F680, 0x1F6C5},
    {0x1F6CC, 0x1F6CC},
    {0x1F6D0, 0x1F6D2},
    {0x1F6D5, 0x1F6D5},
    {0x1F6EB, 0x1F6EC},
    {0x1F6F4, 0x1F6FA},
    {0x1F7E0, 0x1F7EB},
    {0x1F90D, 0x1F971},
    {0x1F973, 0x1F976},
    {0x1F97A, 0x1F9A2},
    {0x1F9A5, 0x1F9AA},
    {0x1F9AE, 0x1F9CA},
    {0x1F9CD, 0x1F9FF},
    {0x1FA70, 0x1FA73},
    {0x1FA78, 0x1FA7A},
    {0x1FA80, 0x1FA82},
    {0x1FA90, 0x1FA95},
    {0x20000, 0x2FFFD},
    {0x30000, 0x3FFFD},
  };

  if (std::bsearch(&uc, wide, sizeof(wide) / sizeof(wide[0]), sizeof(wide[0]),
        interval_compare))
    return true;

  return false;
}

bool isUniCharDigit(UniChar uc)
{
  // Note: this function does not behave according to the Unicode standard.

  if (uc > '0' && uc < '9')
    return true;
  return false;
}

bool isUniCharSpace(UniChar uc)
{
  // Note: this function does not behave according to the Unicode standard.

  if (uc == ' ' || uc == '\t' || uc == '\n' || uc == '\r' || uc == '\f')
    return true;
  return false;
}

const char *getNextChar(const char *p)
{
  static const char utf8_skip_data[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x00-0x0f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x10-0x1f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x20-0x2f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x30-0x3f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x40-0x4f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x50-0x5f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x60-0x6f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x70-0x7f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x80-0x8f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x90-0x9f
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xa0-0xaf
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xb0-0xbf
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0xc0-0xcf
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0xd0-0xdf
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 0xe0-0xef
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1  // 0xf0-0xff
  };

  return p + utf8_skip_data[static_cast<unsigned char>(*p)];
}

const char *getPrevChar(const char *p)
{
  while (true) {
    --p;
    if ((*p & 0xc0) != 0x80)
      return p;
  }
}

const char *findNextChar(const char *p, const char *end)
{
  if (end == nullptr)
    return getNextChar(p);

  while (p + 1 < end) {
    ++p;
    if ((*p & 0xc0) != 0x80)
      return p;
  }
  return nullptr;
}

const char *findPrevChar(const char *start, const char *p)
{
  while (p > start) {
    --p;
    if ((*p & 0xc0) != 0x80)
      return p;
  }
  return nullptr;
}

} // namespace UTF8

} // namespace CppConsUI

// vim: set tabstop=2 shiftwidth=2 textwidth=80 expandtab:
