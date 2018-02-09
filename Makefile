SLUG = DHE-Modules
VERSION = 0.5.0-beta.5

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

GUI_SOURCES = $(wildcard src/gui/*.cpp)
PLUGIN_SOURCES = $(wildcard src/plugin/*.cpp)
MODULE_SOURCES =$(wildcard src/modules/*.cpp src/util/*.cpp)
SOURCES += $(MODULE_SOURCES) $(GUI_SOURCES) $(PLUGIN_SOURCES)

DISTRIBUTABLES +=  $(wildcard LICENSE*) res

include ../../plugin.mk

TEST_SOURCES = $(wildcard test/*.cpp)

TEST_OBJECTS += $(patsubst %, build/%.o, $(TEST_SOURCES)) $(patsubst %, build/%.o, $(MODULE_SOURCES))

build/run-tests: $(TEST_OBJECTS)
	$(CXX) -o $@ $^

test: build/run-tests
	$<

