/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DVDSubtitleParserWebVTT.h"

#include "SubtitlesStyle.h"
#include "cores/VideoPlayer/Interface/TimingConstants.h"
#include "utils/StringUtils.h"
#include "utils/log.h"

CDVDSubtitleParserWebVTT::CDVDSubtitleParserWebVTT(std::unique_ptr<CDVDSubtitleStream>&& pStream,
                                                   const std::string& strFile)
  : CDVDSubtitleParserText(std::move(pStream), strFile, "WebVTT Subtitle Parser")
{
}

CDVDSubtitleParserWebVTT::~CDVDSubtitleParserWebVTT()
{
  Dispose();
}

bool CDVDSubtitleParserWebVTT::Open(CDVDStreamInfo& hints)
{
  if (!CDVDSubtitleParserText::Open())
    return false;

  if (!Initialize())
    return false;

  if (!m_subtitleHandler.Initialize())
    return false;

  char line[1024];

  // Get the first chars to check WebVTT signature / ISOBMFF WebVTT format
  if (m_pStream->Read(line, 10) > 0 && !m_subtitleHandler.CheckSignature(line))
    return false;
  m_pStream->Seek(0, SEEK_SET);

  std::string strLine;

  while (m_pStream->ReadLine(line, sizeof(line)))
  {
    strLine.assign(line);
    m_subtitleHandler.DecodeLine(strLine, &m_subtitleList);
  }

  // "ReadLine" ignore the last empty line of the file and could also be missing,
  // this is required to sign the end of the last Cue
  strLine = "";
  m_subtitleHandler.DecodeLine(strLine, &m_subtitleList);

  for (auto& subData : m_subtitleList)
  {
    KODI::SUBTITLES::subtitleOpts opts;
    opts.useMargins = subData.useMargins;
    opts.marginLeft = subData.marginLeft;
    opts.marginRight = subData.marginRight;
    opts.marginVertical = subData.marginVertical;

    AddSubtitle(subData.text.c_str(), subData.startTime * DVD_TIME_BASE,
                subData.stopTime * DVD_TIME_BASE, &opts);
  }

  m_collection.Add(CreateOverlay());

  return true;
}

void CDVDSubtitleParserWebVTT::Dispose()
{
  CDVDSubtitleParserCollection::Dispose();
}
