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

dep/dheunit/dheunit:
	git submodule update --init --recursive

TEST_SOURCES = $(shell find test -name "*.cpp")

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))
-include $(TEST_OBJECTS:.o=.d)

TEST_RUNNER = build/dheunit

TEST_CXX_FLAGS = -Itest/ -Idep/dheunit

$(TEST_OBJECTS): dep/dheunit/dheunit

$(TEST_OBJECTS): FLAGS += $(TEST_CXX_FLAGS)

$(TEST_RUNNER): $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^

.PHONY: dhetest
dhetest: $(TEST_RUNNER)
	$<

.PHONY: test
test: dhetest


########################################################################
#
# Stage the plugin and run Rack
#
########################################################################

PLUGIN_ZIP_NAME = $(SLUG)-$(VERSION)-$(ARCH).zip
DIST_PLUGIN_ZIP = dist/$(PLUGIN_ZIP_NAME)

STAGING_DIRNAME = .stage
STAGING_DIR = $(realpath .)/$(STAGING_DIRNAME)
STAGING_USER_DIRNAME = rack-user-dir

STAGING_USER_DIR = $(STAGING_DIR)/$(STAGING_USER_DIRNAME)
STAGING_PLUGIN_DIR = $(STAGING_USER_DIR)/plugins-v1
STAGING_PLUGIN_ZIP = $(STAGING_PLUGIN_DIR)/$(PLUGIN_ZIP_NAME)

APP_DIR ?= /Applications
RACK_APP_DIR = $(APP_DIR)/Rack.app
RACK_EXECUTABLE_PATH = $(RACK_APP_DIR)/Contents/MacOS/Rack
RACK_SYSTEM_DIR = $(RACK_APP_DIR)/Contents/Resources

$(STAGING_DIR) $(STAGING_PLUGIN_DIR):
	mkdir -p $@

stage: dist $(STAGING_PLUGIN_DIR)
	cp $(DIST_PLUGIN_ZIP) $(STAGING_PLUGIN_DIR)

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

COMPILATION_DB_ENTRIES := $(patsubst %, build/%.json, $(SOURCES))

$(COMPILATION_DB): $(COMPILATION_DB_ENTRIES)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

build/%.json: %
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

.PHONY: tidy
tidy: $(COMPILATION_DB)
	clang-tidy -header-filter="^src/".* -p=build $(SOURCES)

.PHONY: cleancdb
cleancdb:
	rm -rf $(COMPILATION_DATABASE)

clean: cleancdb

clion: $(OBJECTS) build/dheunit

