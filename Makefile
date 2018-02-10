SLUG = DHE-Modules
VERSION = no-version

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

PLUGIN_SOURCES = $(wildcard src/plugin/*.cpp src/gui/*.cpp)
MODULE_SOURCES = $(wildcard src/modules/*.cpp src/util/*.cpp)
SOURCES = $(MODULE_SOURCES) $(PLUGIN_SOURCES)

DISTRIBUTABLES += $(wildcard LICENSE*) res

include ../../plugin.mk

run: clean all
	make -C ../.. run

MODULE_OBJECTS = $(patsubst %, build/%.o, $(MODULE_SOURCES))

TEST_SOURCES = test/runner/main.cpp $(wildcard test/*.cpp)
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))

$(TEST_OBJECTS): FLAGS+= -I./test

build/test/runner/main: $(TEST_OBJECTS) $(MODULE_OBJECTS)
	$(CXX) -o $@ $^

test: build/test/runner/main
	$<

