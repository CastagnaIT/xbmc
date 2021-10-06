/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "OverlayCodecWebVTT.h"

#include "DVDCodecs/DVDCodecs.h"
#include "DVDOverlayText.h"
#include "DVDStreamInfo.h"
#include "cores/VideoPlayer/Interface/DemuxPacket.h"
#include "utils/log.h"
#include "cores/VideoPlayer/DVDSubtitles/SubtitlesStyle.h"
#include "cores/VideoPlayer/Interface/TimingConstants.h"

#include <sstream>
#include <memory>

COverlayCodecWebVTT::COverlayCodecWebVTT() : CDVDOverlayCodec("WebVTT Subtitle Decoder")
{
  m_pOverlay = nullptr;
}

COverlayCodecWebVTT::~COverlayCodecWebVTT()
{
  Dispose();
}

bool COverlayCodecWebVTT::Open(CDVDStreamInfo& hints, CDVDCodecOptions& options)
{
  Dispose();

  return Initialize() && m_webvttHandler.Initialize();
}

void COverlayCodecWebVTT::Dispose()
{
  if (m_pOverlay)
  {
    m_pOverlay->Release();
    m_pOverlay = nullptr;
  }
}

int COverlayCodecWebVTT::Decode(DemuxPacket* pPacket)
{
  if (!pPacket)
  {
    CLog::Log(LOGERROR, "{} - NOPACCO", __FUNCTION__);
    return OC_ERROR;
  }

  uint8_t* data = pPacket->pData;
  char* start = (char*)data;
  char* end = (char*)(data + pPacket->iSize);

  if (!m_webvttHandler.CheckSignature(start))
      return OC_ERROR;

  std::string strData(start, end);
  CLog::Log(LOGERROR, "{} - DATI QUI {}", __FUNCTION__, strData);

  std::istringstream streamData(strData);
  std::vector<subtitleData> subtitleList;

  std::string line;
  while (std::getline(streamData, line))
  {
    m_webvttHandler.DecodeLine(line, &subtitleList);
  }

  // It is mandatory to send an empty line
  // to mark the end of the last Cue in case it is missing
  m_webvttHandler.DecodeLine("", &subtitleList);

  // Send decoded lines to the renderer
  for (auto& subData : subtitleList)
  {
    KODI::SUBTITLES::subtitleOpts opts;
    opts.useMargins = subData.useMargins;
    opts.marginLeft = subData.marginLeft;
    opts.marginRight = subData.marginRight;
    opts.marginVertical = subData.marginVertical;

    AddSubtitle(subData.text.c_str(), subData.startTime * DVD_TIME_BASE,
                subData.stopTime * DVD_TIME_BASE, &opts);
  }

  return m_pOverlay ? OC_DONE : OC_OVERLAY;
}

void COverlayCodecWebVTT::Reset()
{
  //Dispose();
  //Flush();
}

void COverlayCodecWebVTT::Flush()
{
  /*
  if (m_pOverlay)
  {
    m_pOverlay->Release();
    m_pOverlay = nullptr;
  }
  */
  //Flush only with ISOBFF?
  //FlushSubtitles();
}

CDVDOverlay* COverlayCodecWebVTT::GetOverlay()
{
  if (m_pOverlay)
    return nullptr;
  m_pOverlay = CreateOverlay();
  return m_pOverlay->Acquire();
}
