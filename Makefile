SLUG = DHE-Modules
VERSION = 0.6.0
RACK_DIR ?= ../..

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*/*.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

include $(RACK_DIR)/plugin.mk

run: dist
	cp -R dist/DHE-Modules $(RACK_DIR)/plugins
	make -C $(RACK_DIR) run

MODULE_OBJECTS = $(patsubst %, build/%.o, $(MODULE_SOURCES))

TEST_SOURCES = test/runner/main.cpp $(wildcard test/*.cpp)
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))

$(TEST_OBJECTS): FLAGS+= -I./test

build/test/runner/main: $(TEST_OBJECTS)
	$(CXX) -o $@ $^

test: build/test/runner/main
	$<
