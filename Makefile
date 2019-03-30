SLUG = DHE-Modules
VERSION = 0.6.5
RACK_DIR ?= ../..

FLAGS += -I./include
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*.cpp)

DISTRIBUTABLES += LICENSE.txt svg

include $(RACK_DIR)/plugin.mk

# Above this line: Standard plugin build configuration
########################################################################
# Below this line: Special configuration and targets for Dale




########################################################################
#
# Build and run the tests
#
########################################################################

TEST_SOURCES =  $(wildcard test/*.cpp)
TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))
TESTFLAGS += -Igoogletest/googletest/include/ -Igoogletest/googlemock/include/

$(TEST_OBJECTS): FLAGS += $(TESTFLAGS)

TEST_RUNNER = build/dhe-module-tests

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^ -stdlib=libc++ -Lgoogletest/lib -lgmock_main -lgtest -lgmock

test: all

test: $(TEST_RUNNER)
	$<




########################################################################
#
# Build the compilation database that configures CLion
#
########################################################################

# Tell clang to emit a compilation database entry for each source file
# when building the project target
project: FLAGS += -MJ $@.json

COMPILATION_DATABASE_JSONS := $(patsubst %, %.json, $(OBJECTS) $(TEST_OBJECTS) )

$(COMPILATION_DATABASE_JSONS): $(OBJECTS) $(TEST_OBJECTS)

# Aggregate the compilation database entries
compile_commands.json: $(COMPILATION_DATABASE_JSONS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

# Run as 'make -B project' to force compile all sources
project: compile_commands.json




########################################################################
#
# Install and run in a local directory
#
########################################################################

DEV_INSTALL_DIR = .dev
DEV_PLUGIN_DIR = $(DEV_INSTALL_DIR)/plugins

$(DEV_INSTALL_DIR) $(DEV_PLUGIN_DIR):
	mkdir -p $@

$(DEV_PLUGIN_DIR)/Fundamental: $(DEV_PLUGIN_DIR)
	cp -r $(RACK_USER_DIR)/plugins/Fundamental $(DEV_PLUGIN_DIR)

$(DEV_PLUGIN_DIR)/AudibleInstruments: $(DEV_PLUGIN_DIR)
	cp -r $(RACK_USER_DIR)/plugins/AudibleInstruments $(DEV_PLUGIN_DIR)

dev: dist $(DEV_PLUGIN_DIR) $(DEV_PLUGIN_DIR)/Fundamental $(DEV_PLUGIN_DIR)/AudibleInstruments
	cp dist/$(SLUG)-$(VERSION)-$(ARCH).zip $(DEV_PLUGIN_DIR)

debug: dev
	/Applications/Rack.app/Contents/MacOS/Rack -d -g /Applications/Rack.app/Contents/Resources -l $(realpath $(DEV_INSTALL_DIR))

run: dev
	/Applications/Rack.app/Contents/MacOS/Rack -g /Applications/Rack.app/Contents/Resources -l $(realpath $(DEV_INSTALL_DIR))




########################################################################
#
# Build the SVG files for the GUI
#
########################################################################

gui:
	cd gui && rake install




########################################################################
#
# Utilities
#
########################################################################

# Apply .clang-format to all source files
tidy:
	find src include -name *.h -o -name *.cpp | xargs clang-format -i -style=file

clobber: clean uninstall
	cd gui && rake clobber

fresh: clean test

uninstall:
	rm -rf $(DEV_INSTALL_DIR)

.PHONY: clean clobber fresh gui tidy uninstall

