SLUG = DHE-Modules
VERSION = 0.5.0-beta.5

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES += $(wildcard src/**/*.cpp)

DISTRIBUTABLES +=  $(wildcard LICENSE*) res

include ../../plugin.mk
