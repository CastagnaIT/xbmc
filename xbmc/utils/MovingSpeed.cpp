/*
 *  Copyright (C) 2022 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "MovingSpeed.h"

#include "utils/TimeUtils.h"
#include "utils/log.h"

void UTILS::MOVING_SPEED::MovingSpeed::AddEventConfig(uint32_t eventId,
                                                      float acceleration,
                                                      float maxVelocity,
                                                      uint32_t resetTimeout)
{
  EventCfg eventCfg{acceleration, maxVelocity, resetTimeout};
  m_eventsCfgs.emplace(eventId, eventCfg);
}

void UTILS::MOVING_SPEED::MovingSpeed::AddEventConfig(uint32_t eventId, EventCfg event)
{
  m_eventsCfgs.emplace(eventId, event);
}

void UTILS::MOVING_SPEED::MovingSpeed::Reset()
{
  m_currentEventId = 0;
  for (auto& eventPair : m_eventsCfgs)
  {
    Reset(eventPair.first);
  }
}

void UTILS::MOVING_SPEED::MovingSpeed::Reset(uint32_t eventId)
{
  auto mapIt = m_eventsCfgs.find(eventId);
  if (mapIt == m_eventsCfgs.end())
  {
    CLog::LogF(LOGWARNING, "Cannot reset Event ID {} configuration", eventId);
  }
  else
  {
    EventCfg& event = mapIt->second;
    event.m_currentVelocity = 1.0f;
    event.m_lastFrameTime = 0;
  }
}

float UTILS::MOVING_SPEED::MovingSpeed::GetUpdatedDistance(uint32_t eventId)
{
  auto mapIt = m_eventsCfgs.find(eventId);
  if (mapIt == m_eventsCfgs.end())
  {
    CLog::LogF(LOGERROR, "No event configuration with event ID {}", eventId);
    return 0;
  }
  else
  {
    EventCfg& event = mapIt->second;

    uint32_t currentFrameTime{CTimeUtils::GetFrameTime()};
    uint32_t deltaFrameTime{currentFrameTime - event.m_lastFrameTime};
    float distance{1};

    if (event.m_lastFrameTime != 0 && deltaFrameTime > event.m_resetTimeout)
    {
      // If the delta time exceed the timeout then reset values
      Reset(eventId);
    }
    else if (m_currentEventId != eventId)
    {
      // If the event id is changed then reset values
      Reset(eventId);
    }
    else if (event.m_lastFrameTime != 0)
    {
      // Calculate the new speed based on time so as not to depend on the frame rate
      event.m_currentVelocity += event.m_acceleration * (static_cast<float>(deltaFrameTime) / 1000);

      if (event.m_maxVelocity > 0 && event.m_currentVelocity > event.m_maxVelocity)
        event.m_currentVelocity = event.m_maxVelocity;

      distance = event.m_currentVelocity * (static_cast<float>(deltaFrameTime) / 1000);
    }

    m_currentEventId = eventId;
    event.m_lastFrameTime = currentFrameTime;
    return distance;
  }
}
