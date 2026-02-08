# Include shared feature flags
include $(dir $(realpath $(lastword $(MAKEFILE_LIST))))../../../../../../rules.mk

SRC += secrets.c
