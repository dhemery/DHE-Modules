SLUG = DHE-Modules
VERSION = 0.6.2
RACK_DIR ?= ../..

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/**/*.cpp)  $(wildcard src/*.cpp)

DISTRIBUTABLES += LICENSE.txt res

include $(RACK_DIR)/plugin.mk

# Above this line: Standard plugin build stuff
########################################################################
# Below this line: Targets for Dale to build the gui and test the plugin

TEST_SOURCES = $(shell find test -type f -name '*.cpp')
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))
TEST_RUNNER = build/test-runner

$(TEST_OBJECTS): FLAGS+= -I./test

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^

test: $(TEST_RUNNER)
	$<

RACK_APP = /Applications/Rack.app
RACK_PLUGINS_DIR = ~/Documents/Rack/plugins

run: install
	open $(RACK_APP)

uninstall:
	rm -rf $(MODULE_INSTALL_DIR)

.PHONY: gui
gui:
	$(MAKE) -C gui clobber all


.PHONY: clobber
clobber: clean uninstall
	$(MAKE) -C gui clobber
