SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
include $(SELF_DIR)/file-definitions.mk

DHE_CMAKE_OPTIONS += -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON

DHE_CTEST_OPTIONS += --output-on-failure

include $(SELF_DIR)/cmake-targets.mk
