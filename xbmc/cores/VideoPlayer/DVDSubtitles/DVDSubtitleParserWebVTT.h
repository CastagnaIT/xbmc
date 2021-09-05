/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "DVDSubtitleParser.h"
#include "SubtitlesAdapter.h"
#include "WebVTTHandler.h"

#include <memory>
#include <vector>

class CDVDSubtitleParserWebVTT : public CDVDSubtitleParserText, public CSubtitlesAdapter
{
public:
  CDVDSubtitleParserWebVTT(std::unique_ptr<CDVDSubtitleStream>&& pStream,
                           const std::string& strFile);
  ~CDVDSubtitleParserWebVTT() override;

  bool Open(CDVDStreamInfo& hints) override;
  void Dispose() override;

private:
  WebVTTHandler m_subtitleHandler;
  std::vector<subtitleData> m_subtitleList;
};
