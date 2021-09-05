/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "WebVTTUtils.h"
#include "WebVTTHandler.h"

class ParsableCharArray;


enum sectionISO
{
  WEBVTT_SECTION_ISO_UNDEFINED = 0,
  WEBVTT_SECTION_ISO_VTTC,
  WEBVTT_SECTION_ISO_STTG,
  WEBVTT_SECTION_ISO_PAYL
};

class WebVTTISOHandler: WebVTTHandler
{
public:
  WebVTTISOHandler(){};
  ~WebVTTISOHandler(){};

  bool Initialize();
  bool CheckISOFormatType(const char* buffer);
  void DecodeStream(const char* buffer, double pts, std::vector<subtitleData>* subList);

private:
  void ParseVTTCueBox(ParsableCharArray& sampleData,
                      int remainingCueBoxChars,
                      std::vector<subtitleData>* subList);

  int m_timescale = 1000;
  std::string m_currentCueProps;
  subtitleData m_subtitleData;
};
