RACK_DIR ?= ../..

ALL_DIRS = $(filter %/, $(wildcard plugin/*/ plugin/*/*/ plugin/*/*/*/ plugin/*/*/*/*/))
TEST_DIRS = $(foreach dir,$(ALL_DIRS),$(wildcard $(dir)test/))
SOURCE_DIRS =  $(filter-out $(TEST_DIRS), $(ALL_DIRS))

SOURCES = $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)*.cpp))

INCLUDE_FLAGS = $(foreach dir,$(SOURCE_DIRS),-I$(dir))

FLAGS += $(INCLUDE_FLAGS)
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

DISTRIBUTABLES += LICENSE.txt svg

show:
	@echo ALL_DIRS $(ALL_DIRS)
	@echo TEST_DIRS $(TEST_DIRS)
	@echo SOURCE_DIRS $(SOURCE_DIRS)
	@echo SOURCES $(SOURCES)
	@echo INCLUDE_FLAGS $(INCLUDE_FLAGS)

include $(RACK_DIR)/plugin.mk

# Above this line: Standard plugin build configuration
########################################################################
# Below this line: Special configuration and targets for Dale




########################################################################
#
# Build and run the test
#
########################################################################

TEST_SOURCES = $(foreach dir,$(TEST_DIRS),$(wildcard $(dir)*.cpp))
TEST_OBJECTS = $(patsubst %, build/%.o, $(TEST_SOURCES))
TEST_INCLUDE_FLAGS = \
	$(foreach dir,$(TEST_DIRS),-I$(dir)) \
	-Igoogletest/googletest/include/ \
	-Igoogletest/googlemock/include/

TEST_LDFLAGS += \
	-Lgoogletest/lib \
	-lgmock_main \
	-lgtest \
	-lgmock

ifdef ARCH_LIN
	TESTLDFLAGS += -lpthread
endif

$(TEST_OBJECTS): FLAGS += $(TEST_INCLUDE_FLAGS)

TEST_RUNNER = build/test-runner

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^ $(TEST_LDFLAGS)

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
	$(RACK_EXECUTABLE) $(RACK_FLAGS) -u $(realpath $(DEV_DIR))

debug: dev
	$(RACK_EXECUTABLE) $(RACK_FLAGS) -d -u $(realpath $(DEV_DIR))

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

TIDY_HEADERS = $(shell find plugin -name *.h)
TIDY_SOURCES = $(shell find plugin -name *.cpp)

tidy: project
	clang-tidy $(TIDY_SOURCES)

format:
	clang-format -i -style=file $(TIDY_HEADERS) $(TIDY_SOURCES)

clobber: fresh
	cd gui && rake clobber

fresh: clean undev undb

.PHONY: clobber fresh gui tidy

