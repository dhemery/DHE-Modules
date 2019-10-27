include makefiles/dhe-definitions.mk

FLAGS += $(DHE_INCLUDE_FLAGS)
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=
SOURCES = $(DHE_SOURCE_FILES)
DISTRIBUTABLES += $(DHE_DISTRIBUTABLES)

RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk

include makefiles/dhe-targets.mk
