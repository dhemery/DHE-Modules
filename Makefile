RACK_DIR ?= ../..

FLAGS += -Isrc
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=
SOURCES = $(wildcard src/*.cpp)
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

TEST_SOURCES = $(wildcard \
		test/*.cpp \
		test/*/*.cpp \
		test/*/*/*.cpp \
		)

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))
-include $(TEST_OBJECTS:.o=.d)

TEST_RUNNER = build/testrunner

TEST_CXX_FLAGS += -Itest/

$(TEST_OBJECTS): FLAGS += $(TEST_CXX_FLAGS)

$(TEST_RUNNER): $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^

.PHONY: test
test: $(TEST_RUNNER)
	$<


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

COMPILATION_DATABASE_FILE = compile_commands.json

COMPILATION_DATABASE_JSONS := $(patsubst %, build/%.json, $(SOURCES) $(TEST_SOURCES))

$(COMPILATION_DATABASE_FILE): $(COMPILATION_DATABASE_JSONS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

HEADERS = $(wildcard \
		src/*.h \
		src/*/*.h \
		src/*/*/*.h \
		test/*.h \
		test/*/*.h \
		test/*/*/*.h \
		)

.PHONY: format
format:
	clang-format -i -style=file $(HEADERS) $(SOURCES) $(TEST_SOURCES)

.PHONY: tidy
tidy: $(COMPILATION_DATABASE_FILE)
	clang-tidy -header-filter="^(src|test)/".* -p=build $(SOURCES)

.PHONY: cleancdb
cleancdb:
	rm -rf $(COMPILATION_DATABASE_FILE) $(COMPILATION_DATABASE_JSONS)

clean: cleancdb




########################################################################
#
# General rules
#
########################################################################

build/src/%.json: src/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MJ $@ -c -o build/$^.o $^

build/test/%.json: test/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(TEST_CXX_FLAGS) -MJ $@ -c -o build/$^.o $^
