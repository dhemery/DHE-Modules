SLUG = DHE-Modules
VERSION = 0.6.0
RACK_DIR ?= ../..

RACK_APP = /Applications/Rack.app
RACK_INSTALLED_PLUGINS_DIR = ~/Documents/Rack/plugins
RESOURCES_DIR = res
INSTALL_DIR = $(RACK_INSTALLED_PLUGINS_DIR)/$(SLUG)
CLOBBERABLES = $(INSTALL_DIR) $(RESOURCES_DIR)

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*/*.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

include $(RACK_DIR)/plugin.mk

MODULE_OBJECTS = $(patsubst %, build/%.o, $(MODULE_SOURCES))

TEST_SOURCES = $(wildcard test/runner/*.cpp test/*.cpp)
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))

$(TEST_OBJECTS): FLAGS+= -I./test

build/test/runner/main: $(TEST_OBJECTS)
	$(CXX) -o $@ $^

test: build/test/runner/main
	$<

install: test dist
	rsync -r --delete-after dist/DHE-Modules/ $(INSTALL_DIR)

run: install
	open /Applications/Rack.app

.PHONY: gui
gui:
	cd gui && rake clobber all

clobber: clean
	rm -rf $(CLOBBERABLES)
