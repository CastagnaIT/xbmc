/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "WebVTTUtils.h"
#include "utils/StringUtils.h"
#include "utils/log.h"


WebVTTUtils::WebVTTUtils()
{
}


WebVTTUtils::~WebVTTUtils()
{
}

// Update the instance and reset the position to zero
void WebVTTUtils::Reset(const char* data, int limit)
{
  m_data = data;
  m_limit = limit;
  m_position = 0;
}

// Return the number of chars yet to be read
int WebVTTUtils::CharsLeft()
{
  return m_limit - m_position;
}

// Set the reading offset in the array
void WebVTTUtils::SetPosition(int position)
{
  if (position >= 0 && position <= m_limit)
    m_position = position;
  else
    CLog::Log(LOGERROR, "{} - Position out of range", __FUNCTION__);
}

void WebVTTUtils::SkipChars(int chars)
{
  SetPosition(m_position + chars);
}

// Reads the next four chars as int value
int WebVTTUtils::ReadNextInt()
{
  return (m_data[m_position++] & 0xFF) << 24 | (m_data[m_position++] & 0xFF) << 16 |
         (m_data[m_position++] & 0xFF) << 8 | (m_data[m_position++] & 0xFF);
}

// Reads the next four chars as a string value
std::string WebVTTUtils::ReadNextChars()
{
  std::string subChars(m_data + m_position, 4);
  return subChars;
}

// Returns the current offset in the array
int WebVTTUtils::GetPosition()
{
  return m_position;
}
