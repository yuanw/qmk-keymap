# Include shared rules
ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include ${ROOT_DIR}../../../../../../rules.mk

# Imprint-specific rules
TAP_DANCE_ENABLE ?= no

SRC += ${ROOT_DIR}../../../../../../secrets.c
