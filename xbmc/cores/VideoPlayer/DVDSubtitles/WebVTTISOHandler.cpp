/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "WebVTTISOHandler.h"

#include "utils/CharsetConverter.h"
#include "utils/StringUtils.h"
#include "utils/log.h"

#include <cstring>

#define GET_ATOM_INT(c1, c2, c3, c4) \
  (((uint32_t(c1)) << 24) | ((uint32_t(c2)) << 16) | ((uint32_t(c3)) << 8) | ((uint32_t(c4))))

namespace
{
// ISOBMFF MP4 WebVTT atoms
constexpr int ISO_BOX_HEADER_SIZE = 8;
constexpr int ISO_BOX_BRAND_ISOM = GET_ATOM_INT('i', 's', 'o', 'm');
constexpr int ISO_BOX_BRAND_DASH = GET_ATOM_INT('d', 'a', 's', 'h');
constexpr int ISO_BOX_BRAND_SMOOTH = GET_ATOM_INT('s', 'm', 'o', 'o');
constexpr int ISO_BOX_TYPE_FTYP = GET_ATOM_INT('f', 't', 'y', 'p'); // File Type Box
constexpr int ISO_BOX_TYPE_STYP = GET_ATOM_INT('s', 't', 'y', 'p'); // Segment Type Box
constexpr int ISO_BOX_TYPE_VTTC = GET_ATOM_INT('v', 't', 't', 'c'); // VTTCueBox
constexpr int ISO_BOX_TYPE_STTG = GET_ATOM_INT('s', 't', 't', 'g'); // CueSettingsBox
constexpr int ISO_BOX_TYPE_PAYL = GET_ATOM_INT('p', 'a', 'y', 'l'); // CuePayloadBox
constexpr int ISO_BOX_TYPE_VTTE = GET_ATOM_INT('v', 't', 't', 'e'); // VTTEmptyCueBox
} // namespace

bool WebVTTISOHandler::Initialize()
{
  if (!WebVTTHandler::Initialize())
    return false;
}

bool WebVTTISOHandler::CheckISOFormatType(const char* buffer)
{
  // Check if the WebVTT data is in ISOBMFF format type (ISO/IEC 14496-30:2014)
  // MP4-Encapsulated Subtitles
  ParsableCharArray sampleData;
  sampleData.Reset(buffer, sizeof(buffer));
  if (sampleData.CharsLeft() >= ISO_BOX_HEADER_SIZE)
  {
    int boxType = sampleData.ReadNextInt();
    if (boxType == ISO_BOX_TYPE_FTYP || boxType == ISO_BOX_TYPE_STYP)
    {
      int brand = sampleData.ReadNextInt();
      if (brand != ISO_BOX_BRAND_ISOM && brand != ISO_BOX_BRAND_DASH &&
          brand != ISO_BOX_BRAND_SMOOTH)
        CLog::Log(LOGWARNING, "{} - Brand code {} not identified", __FUNCTION__, brand);
      // TODO: To check track to know if it is a "WEBVTT" format (maybe "stsd" atom)
      // TODO: Get timescale value from "mdhd" atom (this is contained in ftyp->moov->trak->mdia->mdhd)
      // to be set to m_timescale should be provided the first time only
      m_timescale = 1000;
      return true;
    }
  }

  CLog::Log(LOGINFO, "{} - The stream is not a WebVTT ISOBMFF format type", __FUNCTION__);
  return false;
}

void WebVTTISOHandler::DecodeStream(const char* buffer,
                                    double pts,
                                    std::vector<subtitleData>* subList)
{
  // This MP4 WebVTT decoder has been inspired from ExoPlayer sources
  // ISOBMFF WebVTT format type that differ from regular WebVTT
  // it make use of boxes (first 4 byte speficy the size + next 4 byte specify the type)
  // and Start/Stop subtitle times info are not included,
  // the Start time is the pts provided from the decoder (when a VTTCueBox is decoded)
  // the Stop time is defined by the pts of the next VTTEmptyCueBox or VTTCueBox decoded

  ParsableCharArray sampleData;

  sampleData.Reset(buffer, strlen(buffer));

  while (sampleData.CharsLeft() > 0)
  {
    if (sampleData.CharsLeft() < ISO_BOX_HEADER_SIZE)
    {
      CLog::Log(LOGWARNING, "{} - Incomplete box header found", __FUNCTION__);
      break;
    }

    int boxSize = sampleData.ReadNextInt();
    int boxType = sampleData.ReadNextInt();
    if (boxType == ISO_BOX_TYPE_VTTC)
    {
      // TODO: if previous subtitle has no stop time set the stop time of this pts
      ParseVTTCueBox(sampleData, boxSize - ISO_BOX_HEADER_SIZE, subList);
    }
    else if (boxType == ISO_BOX_TYPE_VTTE)
    {
      // TODO: We use this to set the stop time of previous added subtitle
      sampleData.SkipChars(boxSize - ISO_BOX_HEADER_SIZE);
    }
    else
    {
      // Skip unsupported box types
      sampleData.SkipChars(boxSize - ISO_BOX_HEADER_SIZE);
    }
  }
}

void WebVTTISOHandler::ParseVTTCueBox(ParsableCharArray& sampleData,
                                   int remainingCueBoxChars,
                                   std::vector<subtitleData>* subList)
{
  while (remainingCueBoxChars > 0)
  {
    if (remainingCueBoxChars < ISO_BOX_HEADER_SIZE)
    {
      CLog::Log(LOGWARNING, "{} - Incomplete VTT Cue box header found", __FUNCTION__);
      return;
    }
    int boxSize = sampleData.ReadNextInt();
    int boxType = sampleData.ReadNextInt();
    remainingCueBoxChars -= ISO_BOX_HEADER_SIZE;
    int payloadLength = boxSize - ISO_BOX_HEADER_SIZE;
    std::string boxPayload(sampleData.GetData() + sampleData.GetPosition(), payloadLength);
    sampleData.SkipChars(payloadLength);
    remainingCueBoxChars -= payloadLength;

    if (boxType == ISO_BOX_TYPE_STTG)
    {
      // Store the cue properties
      m_currentCueProps = boxPayload;
    }
    else if (boxType == ISO_BOX_TYPE_PAYL)
    {
      // Collect and convert all subtitle text lines
      if (!m_subtitleData.text.empty())
        m_subtitleData.text += "\n";

      // Convert tags and apply only default CSS Styles converted
      ConvertLine(boxPayload);

      // We need to convert the cue properties right here,
      // when we have the first subtitle text line
      if (m_subtitleData.text.empty())
      {
        m_subtitleData = subtitleData();
        ConvertCueProperties(m_currentCueProps, boxPayload, m_subtitleData);
      }

      m_subtitleData.text += boxPayload;
    }
    else
    {
      // Other VTTCueBox childrens are not supported
    }
  }

  //TODO add to subList the subtitle data
}
