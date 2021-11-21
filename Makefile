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

TEST_SOURCES = $(shell find test -name "*.cpp")

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))
-include $(TEST_OBJECTS:.o=.d)

TEST_RUNNER = build/dheunit

RACK_INCLUDES = -I$(RACK_DIR)/include -I$(RACK_DIR)/dep/include
TEST_CXX_FLAGS := $(filter-out $(RACK_INCLUDES),$(FLAGS)) -Itest -I$(DHEUNIT_SRC)

$(TEST_OBJECTS): $(DHEUNIT_INCLUDE_DIR)
$(TEST_OBJECTS): FLAGS := $(TEST_CXX_FLAGS)

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

APP_DIR ?= /Applications
RACK_APP_DIR = $(APP_DIR)/VCV\ Rack\ 2.app
RACK_EXECUTABLE_PATH = $(RACK_APP_DIR)/Contents/MacOS/Rack
RACK_SYSTEM_DIR = $(RACK_APP_DIR)/Contents/Resources

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

COMPILATION_DB = compile_commands.json

COMPILATION_DB_ENTRIES := $(patsubst %, build/%.json, $(SOURCES) $(TEST_SOURCES))

$(COMPILATION_DB): $(COMPILATION_DB_ENTRIES)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

setup: $(COMPILATION_DB)

.PHONY: cleancdb
cleancdb:
	rm -rf $(COMPILATION_DB)

clean: cleancdb

build/src/%.json: src/%
	@mkdir -p $(@D)
	clang $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

build/test/%.json: test/%
	@mkdir -p $(@D)
	clang $(TEST_CXX_FLAGS) -MJ $@ -c -o build/$^.o $^

.PHONY: tidy
tidy: $(COMPILATION_DB)
	clang-tidy -p=build $(SOURCES) $(TEST_SOURCES)


IWYU := include-what-you-use
IWYU += -Xiwyu --quoted_includes_first
IWYU += -Xiwyu --mapping_file=.iwyu.libcxx.yaml
IWYU += -Xiwyu --transitive_includes_only

iwyu:
	$(MAKE) -Bi CXX='$(IWYU)' $(OBJECTS) $(TEST_OBJECTS)

check: tidy iwyu

clion: $(OBJECTS) $(TEST_RUNNER)
