DHE_SOURCE_DIRS += plugin

DHE_SOURCE_FILES = $(foreach dir, $(DHE_SOURCE_DIRS), $(wildcard $(dir)/*.cpp))
DHE_INCLUDE_FLAGS = -Iplugin

DHE_DISTRIBUTABLES = LICENSE.txt presets svg
