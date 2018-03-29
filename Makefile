SLUG = DHE-Modules
VERSION = 0.6.1
RACK_DIR ?= ../..

RESOURCE_DIR = gui/res

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*/*.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) $(RESOURCE_DIR)

include $(RACK_DIR)/plugin.mk

MODULE_OBJECTS = $(patsubst %, build/%.o, $(SOURCES))

TEST_SOURCES = $(wildcard test/runner/*.cpp test/*.cpp)
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))

$(TEST_OBJECTS): FLAGS+= -I./test

build/test/runner/main: $(TEST_OBJECTS)
	$(CXX) -o $@ $^

test: build/test/runner/main
	$<

RACK_APP = /Applications/Rack.app
RACK_PLUGINS_DIR = ~/Documents/Rack/plugins
MODULE_INSTALL_DIR = $(RACK_PLUGINS_DIR)/$(SLUG)

install: test dist
	rsync -r --delete-after dist/DHE-Modules/ $(MODULE_INSTALL_DIR)

run: install
	open $(RACK_APP)

uninstall:
	rm -rf $(MODULE_INSTALL_DIR)

.PHONY: gui
gui:
	$(MAKE) -C gui clobber all
