/*
 *  Copyright (C) 2016-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "pvr/guilib/PVRGUIActionsChannels.h"
#include "pvr/guilib/PVRGUIActionsClients.h"
#include "pvr/guilib/PVRGUIActionsDatabase.h"
#include "pvr/guilib/PVRGUIActionsEPG.h"
#include "pvr/guilib/PVRGUIActionsParentalControl.h"
#include "pvr/guilib/PVRGUIActionsPlayback.h"
#include "pvr/guilib/PVRGUIActionsPowerManagement.h"
#include "pvr/guilib/PVRGUIActionsRecordings.h"
#include "pvr/guilib/PVRGUIActionsTimers.h"
#include "pvr/guilib/PVRGUIActionsUtils.h"

namespace PVR
{
class CPVRGUIActions : public CPVRGUIActionsChannels,
                       public CPVRGUIActionsClients,
                       public CPVRGUIActionsDatabase,
                       public CPVRGUIActionsEPG,
                       public CPVRGUIActionsParentalControl,
                       public CPVRGUIActionsPlayback,
                       public CPVRGUIActionsPowerManagement,
                       public CPVRGUIActionsRecordings,
                       public CPVRGUIActionsTimers,
                       public CPVRGUIActionsUtils
{
public:
  CPVRGUIActions() = default;
  virtual ~CPVRGUIActions() = default;
};

} // namespace PVR
