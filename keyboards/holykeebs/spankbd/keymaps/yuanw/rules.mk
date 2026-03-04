# Include shared feature flags
include $(dir $(realpath $(lastword $(MAKEFILE_LIST))))../../../../../rules.mk

# Dual TPS43 pointing devices
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = azoteq_iqs5xx
SPLIT_POINTING_ENABLE = yes
POINTING_DEVICE_COMBINED = yes
