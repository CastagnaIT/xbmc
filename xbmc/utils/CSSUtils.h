/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

class CSSUtils
{
public:
  /*! \brief Escape a CSS string
      \param str the string to be escaped
  */
  static void Escape(std::string& str);
};
