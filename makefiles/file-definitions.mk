DHE_SOURCE_DIRS += plugin/common/src/config
DHE_SOURCE_DIRS += plugin/common/src/controls
DHE_SOURCE_DIRS += plugin/modules/
DHE_SOURCE_DIRS += plugin/modules/*

DHE_SOURCE_FILES = $(foreach dir, $(DHE_SOURCE_DIRS), $(wildcard $(dir)/*.cpp))
DHE_INCLUDE_FLAGS = -Iplugin/common/include

DHE_DISTRIBUTABLES = LICENSE.txt presets svg
