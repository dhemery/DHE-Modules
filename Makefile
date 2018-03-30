SLUG = DHE-Modules
VERSION = 0.6.1
RACK_DIR ?= ../..

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*/*.cpp)

DISTRIBUTABLES += LICENSE.txt gui/res

include $(RACK_DIR)/plugin.mk

TEST_SOURCES = $(wildcard test/runner/*.cpp test/*.cpp)
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))
TEST_RUNNER = build/test-runner

$(TEST_OBJECTS): FLAGS+= -I./test

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^

test: $(TEST_RUNNER)
	$<

RACK_APP = /Applications/Rack.app
RACK_PLUGINS_DIR = ~/Documents/Rack/plugins
MODULE_INSTALL_DIR = $(RACK_PLUGINS_DIR)/$(SLUG)

install: dist
	rsync -r --delete-after dist/DHE-Modules/ $(MODULE_INSTALL_DIR)

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

