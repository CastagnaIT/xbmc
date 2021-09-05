/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

class WebVTTUtils
{
public:
  WebVTTUtils();
  ~WebVTTUtils();

  void Reset(const char* data, int limit);
  int CharsLeft();
  void SetPosition(int position);
  void SkipChars(int chars);
  int ReadNextInt();
  std::string ReadNextChars();
  int GetPosition();
  const char* GetData() { return m_data; };

private:
  const char* m_data;
  int m_position = 0;
  int m_limit = 0;
};
