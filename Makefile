RACK_DIR ?= ../..

FLAGS += -I./include
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard \
			src/*.cpp \
			src/modules/*.cpp \
			src/modules/*/*.cpp \
			src/panels/*/*.cpp \
			src/panels/*.cpp \
			src/util/*.cpp \
			)

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

TEST_SOURCES =  $(wildcard \
					test/*.cpp \
					)
TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))

TESTFLAGS += -Igoogletest/googletest/include/ -Igoogletest/googlemock/include/
TESTLDFLAGS += -Lgoogletest/lib -lgmock_main -lgtest -lgmock

ifdef ARCH_LIN
	TESTLDFLAGS += -lpthread
endif

$(TEST_OBJECTS): FLAGS += $(TESTFLAGS)

TEST_RUNNER = build/test-runner

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

test: all

test: $(TEST_RUNNER)
	$<

.PHONY: test




########################################################################
#
# Build the compilation database that configures CLion
#
########################################################################

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES) $(TEST_SOURCES) )

build/src/%.json: src/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

build/test/%.json: test/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -MJ $@ -c -o build/$^.o $^

$(COMPILATION_DATABASE_FILE): $(COMPILATION_DATABASE_JSONS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

project: $(COMPILATION_DATABASE_FILE)

undb:
	rm -rf $(COMPILATION_DATABASE_FILE) $(COMPILATION_DATABASE_JSONS)

.PHONY: undb




########################################################################
#
# Install and run in a local directory
#
########################################################################

PLUGIN_ZIP_NAME = $(SLUG)-$(VERSION)-$(ARCH).zip
DIST_PLUGIN_ZIP = dist/$(PLUGIN_ZIP_NAME)
DEV_DIR = .dev
DEV_PLUGIN_DIR = $(DEV_DIR)/plugins-v1
DEV_PLUGIN_ZIP = $(DEV_PLUGIN_DIR)/$(PLUGIN_ZIP_NAME)

RACK_APP_DIR ?= /Applications
RACK_APP = $(RACK_APP_DIR)/Rack.app

RACK_EXECUTABLE = $(RACK_APP)/Contents/MacOS/Rack
RACK_SYSTEM_DIR = $(RACK_APP)/Contents/Resources

$(DEV_DIR) $(DEV_PLUGIN_DIR):
	mkdir -p $@

$(DEV_PLUGIN_ZIP): dist $(DEV_PLUGIN_DIR)
	cp $(DIST_PLUGIN_ZIP) $(DEV_PLUGIN_DIR)

unplug:
	rm -rf $(DEV_PLUGIN_DIR)

undev:
	rm -rf $(DEV_DIR)

dev: $(DEV_PLUGIN_ZIP)

run: dev
	$(RACK_EXECUTABLE) $(RACK_FLAGS) -s $(RACK_SYSTEM_DIR) -u $(realpath $(DEV_DIR))

debug: dev
	$(RACK_EXECUTABLE) $(RACK_FLAGS) -d -s $(RACK_SYSTEM_DIR) -u $(realpath $(DEV_DIR))

.PHONY: unplug undev run debug




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

HEADERS = $(shell find include -name *.h)

tidy: project
	clang-tidy $(SOURCES)

format:
	clang-format -i -style=file $(SOURCES) $(HEADERS)

clobber: fresh
	cd gui && rake clobber

fresh: clean undev undb

.PHONY: clobber fresh gui tidy

