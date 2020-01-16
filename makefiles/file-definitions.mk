DHE_SOURCE_DIRS += src

DHE_SOURCE_FILES = $(foreach dir, $(DHE_SOURCE_DIRS), $(wildcard $(dir)/*.cpp))
DHE_INCLUDE_FLAGS = -Isrc

DHE_DISTRIBUTABLES = LICENSE.txt presets svg
