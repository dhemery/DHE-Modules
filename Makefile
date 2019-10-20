RACK_DIR ?= ../..

DHE_FILE_PATTERNS = \
	plugin/ \
	plugin/*/ \
	plugin/*/*/ \
	plugin/*/*/*/ \
	plugin/*/*/*/*/ \

DHE_FILES = $(sort $(wildcard $(DHE_FILE_PATTERNS)))
DHE_SOURCES = $(filter %.cpp, $(DHE_FILES))
DHE_HEADERS = $(filter %.h, $(DHE_FILES))
DHE_DIRS = $(filter %/, $(DHE_FILES))


DHE_TEST_DIRS =  $(filter %/test/, $(DHE_DIRS))
DHE_TEST_SOURCE_PATTERN = $(foreach dir, $(DHE_TEST_DIRS), $(dir)%.cpp)
DHE_TEST_SOURCES = $(filter $(DHE_TEST_SOURCE_PATTERN), $(DHE_SOURCES))
DHE_PRODUCTION_DIRS =  $(filter-out %/test/, $(DHE_DIRS))
DHE_PRODUCTION_SOURCES = $(filter-out $(DHE_TEST_SOURCE_PATTERN), $(DHE_SOURCES))

SOURCES = $(DHE_PRODUCTION_SOURCES)

DISTRIBUTABLES += LICENSE.txt svg

FLAGS += $(foreach dir,$(DHE_PRODUCTION_DIRS),-I$(dir))
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

include $(RACK_DIR)/plugin.mk




# Above this line: Standard plugin build configuration
########################################################################
# Below this line: Special configuration and targets for Dale




########################################################################
#
# Build and run the tests
#
########################################################################

TEST_DIR = .test

cmake:
	cmake plugin -B $(TEST_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build $(TEST_DIR)

test: cmake
	cd $(TEST_DIR) && ctest --progress --output-on-failure && cd -

clean-test:
	rm -rf $(TEST_DIR)

clean: clean-test

.PHONY: ctest cmake clean-test




########################################################################
#
# Analyze and format the code
#
########################################################################

format:
	clang-format -i -style=file $(DHE_HEADERS) $(DHE_SOURCES)

tidy: cmake
	clang-tidy -p=$(TEST_DIR) $(DHE_PRODUCTION_SOURCES)

.PHONY: format tidy




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

dev: $(DEV_PLUGIN_ZIP)

clean-dev:
	rm -rf $(DEV_DIR)

run: dev
	$(RACK_EXECUTABLE) $(RACK_FLAGS) -u $(realpath $(DEV_DIR))

debug: dev
	$(RACK_EXECUTABLE) $(RACK_FLAGS) -d -u $(realpath $(DEV_DIR))

clean: clean-dev

.PHONY: run debug clean-dev




########################################################################
#
# Build the SVG files for the GUI
#
########################################################################

gui:
	cd gui && rake install

.PHONY: gui



########################################################################
#
# Cleanup
#
########################################################################

clobber: clean
	cd gui && rake clobber

.PHONY: clobber gui

