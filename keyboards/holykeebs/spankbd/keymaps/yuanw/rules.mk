# Include shared feature flags
include $(dir $(realpath $(lastword $(MAKEFILE_LIST))))../../../../../rules.mk

# Dual TPS43 pointing devices
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = azoteq_iqs5xx
SPLIT_POINTING_ENABLE = yes
POINTING_DEVICE_COMBINED = yes
SERIAL_DRIVER = vendor

# Right side TPS43 needs 180° rotation; left side uses default orientation
ifeq ($(strip $(SIDE)), right)
    OPT_DEFS += -DAZOTEQ_IQS5XX_ROTATION_180
endif
