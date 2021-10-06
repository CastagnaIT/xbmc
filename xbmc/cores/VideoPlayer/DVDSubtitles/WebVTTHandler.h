/*
 *  Copyright (C) 2005-2021 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "guilib/GUIColorManager.h"
#include "utils/RegExp.h"

#include <map>
#include <stdio.h>
#include <string>
#include <vector>

class ParsableCharArray;

enum flagTags
{
  FLAG_TAG_BOLD = 0,
  FLAG_TAG_ITALIC,
  FLAG_TAG_UNDERLINE,
  FLAG_TAG_CLASS,
  FLAG_TAG_COLOR,
  FLAG_TAG_COUNT
};

enum textAlignment
{
  ALIGN_TOP_LEFT = 0,
  ALIGN_TOP_CENTER,
  ALIGN_TOP_RIGHT,
  ALIGN_SUB_LEFT,
  ALIGN_SUB_CENTER,
  ALIGN_SUB_RIGHT
};

enum webwttAlign
{
  WEBVTT_ALIGN_AUTO = 0,
  WEBVTT_ALIGN_LEFT,
  WEBVTT_ALIGN_CENTER,
  WEBVTT_ALIGN_RIGHT,
  WEBVTT_ALIGN_START,
  WEBVTT_ALIGN_END
};

enum webwttVAlign
{
  WEBVTT_VALIGN_HORIZONTAL = 0,
  WEBVTT_VALIGN_VERTICAL_RL,
  WEBVTT_VALIGN_VERTICAL_LR,
};

struct webvttAutoValue
{
  double value = 0;
  bool isAuto = true;
};

struct webvttCueSettings
{
  std::string id;
  std::string regionId;
  webwttVAlign verticalAlign;
  bool snapToLines;
  webvttAutoValue line;
  webvttAutoValue position;
  webwttAlign positionAlign;
  int size;
  webwttAlign align;
};

struct subtitleData
{
  std::string text;
  webvttCueSettings cueSettings;
  double startTime;
  double stopTime;
  bool useMargins;
  int marginLeft;
  int marginRight;
  int marginVertical;
  textAlignment textAlign;
};

enum section
{
  WEBVTT_SECTION_UNDEFINED = 0,
  WEBVTT_SECTION_STYLE,
  WEBVTT_SECTION_REGION,
  WEBVTT_SECTION_CUE,
  WEBVTT_SECTION_CUE_TEXT,
  WEBVTT_SECTION_CUE_METADATA,
  WEBVTT_SECTION_NOTE
};

enum webvttSelector
{
  WEBVTT_SELECTOR_ANY = 0,
  WEBVTT_SELECTOR_ID,
  WEBVTT_SELECTOR_TYPE,
  WEBVTT_SELECTOR_CLASS,
  WEBVTT_SELECTOR_UNSUPPORTED
};

struct webvttCssStyle
{
  webvttSelector selectorType;
  std::string selectorName;
  std::string color;
  bool isFontBold = false;
  bool isFontItalic = false;
  bool isFontUnderline = false;
};

struct findCssStyleName : std::unary_function<webvttCssStyle, bool>
{
  std::string m_name;
  findCssStyleName(std::string name) : m_name(name) {}
  bool operator()(const webvttCssStyle& other) const { return other.selectorName == m_name; }
};

class WebVTTHandler
{
public:
  WebVTTHandler(){};
  ~WebVTTHandler(){};

  /*!
  * \brief Prepare the handler to the decoding
  */
  bool Initialize();

  /*!
  * \brief Verify the validity of the WebVTT signature
  */
  bool CheckSignature(const char* buffer);

  /*!
  * \brief Decode a line of the WebVTT file data
  * \param line The line to decode
  * \param subList The list to be filled with decoded subtitles
  */
  void DecodeLine(std::string line, std::vector<subtitleData>* subList);

protected:
  void CalculateTextPosition(std::string& subtitleText, subtitleData& subData);

private:
  bool IsCueLine(std::string& line);
  void GetCueData(std::string& cueText);
  std::string GetCueSettingValue(std::string propName, std::string& text, std::string defaultValue);
  void GetCueSettings(std::string& cueSettings, subtitleData& subData);
  std::string GetCueCssValue(std::string cssPropName, std::string& line);
  void AddDefaultCssClasses();
  void ConvertSubtitle(std::string& text);
  void InsertCssStyleStartTag(std::string& tagName,
                              std::string& text,
                              int& pos,
                              int flagTags[],
                              std::map<std::string, std::string>& cssTagsOpened);
  void InsertCssStyleCloseTag(std::string& tagName,
                              std::string& text,
                              int& pos,
                              int flagTags[],
                              std::map<std::string, std::string>& cssTagsOpened,
                              webvttCssStyle& baseStyle);
  bool GetBaseStyle(webvttCssStyle& style);

  std::string m_previousLines[3];
  bool m_overrideStyle;
  bool m_overridePositions;
  section m_currentSection;
  subtitleData m_subtitleData;
  uint64_t m_mpegTsOffset = 0;
  CRegExp m_cueTimeRegex;
  std::map<std::string, CRegExp> m_cuePropsMapRegex;
  CGUIColorManager m_colorManager;
  CRegExp m_tagsRegex;
  CRegExp m_cueCssTagRegex;
  std::map<std::string, CRegExp> m_cueCssStyleMapRegex;
  std::vector<std::string> m_cueCurrentCssStyleSelectors;
  webvttCssStyle m_feedCssStyle;
  std::vector<webvttCssStyle> m_cueCssStyles;
};
