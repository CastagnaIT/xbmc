/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "DebugInfo.h"
#include "OverlayRenderer.h"
#include "cores/VideoPlayer/DVDSubtitles/SubtitlesAdapter.h"

#include <string>

class CDebugRenderer
{
public:
  CDebugRenderer();
  virtual ~CDebugRenderer();
  void Configure();
  void SetInfo(DEBUG_INFO_PLAYER& info);
  void SetInfo(DEBUG_INFO_VIDEO& video, DEBUG_INFO_RENDER& render);
  void Render(CRect& src, CRect& dst, CRect& view);
  void Flush();

protected:
  class CRenderer : public OVERLAY::CRenderer
  {
  public:
    CRenderer();
    void Render(int idx) override;
    KODI::SUBTITLES::subtitlesStyle CreateSubtitlesStyle();
  };

  std::string m_strDebug[6];
  CRenderer m_overlayRenderer;

private:
  CSubtitlesAdapter* m_adapter;
  bool m_isConfigured = false;
};
