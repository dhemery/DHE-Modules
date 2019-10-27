SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(SELF_DIR)/dhe-definitions.mk
include $(SELF_DIR)/dhe-targets.mk
