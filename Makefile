RACK_DIR ?= ../..

FLAGS += -Isrc
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=
SOURCES = $(shell find src -name "*.cpp")
DISTRIBUTABLES += LICENSE.txt presets svg

include $(RACK_DIR)/plugin.mk




# Above this line: Standard plugin build configuration
########################################################################
# Below this line: Special configuration and targets for Dale




########################################################################
#
# Build and run the tests
#
########################################################################

DHEUNIT_SRC = dheunit
DHEUNIT_INCLUDE_DIR = $(DHEUNIT_SRC)/dheunit

$(DHEUNIT_INCLUDE_DIR):
	git submodule update --init --recursive

RACK_INCLUDES = -I$(RACK_DIR)/include -I$(RACK_DIR)/dep/include
TEST_INCLUDES =  -Itest -I$(DHEUNIT_SRC)
TEST_CXXFLAGS = $(filter-out $(RACK_INCLUDES),$(CXXFLAGS)) $(TEST_INCLUDES) 

TEST_SOURCES = $(shell find test -name "*.cpp")

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))
-include $(TEST_OBJECTS:.o=.d)

$(TEST_OBJECTS): $(DHEUNIT_INCLUDE_DIR)
$(TEST_OBJECTS): CXXFLAGS := $(TEST_CXXFLAGS)

TEST_RUNNER = build/dheunit

$(TEST_RUNNER): $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^

.PHONY: test vtest

test: $(TEST_RUNNER)
	$<

vtest: $(TEST_RUNNER)
	$< --verbose



########################################################################
#
# Stage the plugin and run Rack
#
########################################################################

PLUGIN_ZIP_NAME = $(SLUG)-$(VERSION)-mac.vcvplugin
DIST_PLUGIN_ZIP = dist/$(PLUGIN_ZIP_NAME)

STAGING_DIRNAME = .stage
STAGING_DIR = $(realpath .)/$(STAGING_DIRNAME)
STAGING_USER_DIRNAME = rack-user-dir

STAGING_USER_DIR = $(STAGING_DIR)/$(STAGING_USER_DIRNAME)
STAGING_PLUGIN_DIR = $(STAGING_USER_DIR)/plugins
STAGING_PLUGIN_ZIP = $(STAGING_PLUGIN_DIR)/$(PLUGIN_ZIP_NAME)

RACK_EXECUTABLE_PATH = $(RACK_APP)/Contents/MacOS/Rack

$(STAGING_DIR) $(STAGING_PLUGIN_DIR):
	mkdir -p $@

stage: dist $(STAGING_PLUGIN_DIR)
	cp $(DIST_PLUGIN_ZIP) $(STAGING_PLUGIN_DIR)
	cp -r .testing/* $(STAGING_USER_DIR)

clean-stage:
	rm -rf $(STAGING_DIRNAME)

run: stage
	$(RACK_EXECUTABLE_PATH) -u $(STAGING_USER_DIR)

clean: clean-stage

.PHONY: run clean-stage




########################################################################
#
# Analyze and format the code
#
########################################################################

HEADERS = $(shell find src test -name "*.h")

.PHONY: format
format:
	clang-format -i -style=file $(HEADERS) $(SOURCES) $(TEST_SOURCES)

COMPILATION_DB_PLUGIN_ENTRIES := $(patsubst %, build/%.json, $(SOURCES))

COMPILATION_DB_TEST_ENTRIES := $(patsubst %, build/%.json, $(TEST_SOURCES))
$(COMPILATION_DB_TEST_ENTRIES): CXXFLAGS := $(TEST_CXXFLAGS)

COMPILATION_DB_ENTRIES := $(COMPILATION_DB_PLUGIN_ENTRIES) $(COMPILATION_DB_TEST_ENTRIES)

COMPILATION_DB = compile_commands.json

$(COMPILATION_DB): $(COMPILATION_DB_ENTRIES)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

setup: $(COMPILATION_DB)

.PHONY: cleansetup
cleansetup:
	rm -rf $(COMPILATION_DB) $(COMPILATION_DB_ENTRIES)

clean: cleansetup

build/%.json: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

.PHONY: tidy
tidy: $(COMPILATION_DB)
	clang-tidy -p=build $(SOURCES) $(TEST_SOURCES)
