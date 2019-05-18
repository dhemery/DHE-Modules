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
					src/modules/components/*.cpp \
					src/modules/controls/*.cpp \
					src/util/*.cpp \
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

PLUGIN_ZIP_NAME = $(SLUG)-$(VERSION)-$(ARCH).zip
DIST_PLUGIN_ZIP = dist/$(PLUGIN_ZIP_NAME)
DEV_DIR = .dev
DEV_PLUGIN_DIR = $(DEV_DIR)/plugins
DEV_PLUGIN_ZIP = $(DEV_PLUGIN_DIR)/$(PLUGIN_ZIP_NAME)

RACK_EXECUTABLE = $(RACK_APP_DIR)/Rack.app/Contents/MacOS/Rack
RACK_SYSTEM_DIR = $(RACK_APP_DIR)/Rack.app/Contents/Resources

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

debug: RACK_FLAGS += -d

debug: run




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

clobber: clean
	cd gui && rake clobber

fresh: clean undev test

.PHONY: clean clobber fresh gui tidy uninstall

