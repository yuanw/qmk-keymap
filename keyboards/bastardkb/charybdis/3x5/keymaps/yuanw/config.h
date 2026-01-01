#pragma once

#include "config_yuanw.h"

#ifndef __arm__
#define NO_ACTION_ONESHOT
#endif

// Charybdis-specific features
#ifdef POINTING_DEVICE_ENABLE
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER PNT
#endif
