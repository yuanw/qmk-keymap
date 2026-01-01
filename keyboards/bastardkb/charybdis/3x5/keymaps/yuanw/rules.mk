# Include shared rules
ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include ${ROOT_DIR}../../../../../../rules.mk

# Charybdis-specific rules
COMBO_ENABLE = yes

SRC += ${ROOT_DIR}../../../../../../secrets.c
