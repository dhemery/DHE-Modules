RACK_DIR ?= ../..

SOURCE_DIRS = \
	plugin \
	plugin/config \
	plugin/controls \
	plugin/components \
	plugin/modules \
	plugin/modules/ranger \
	plugin/modules/gator \
	plugin/modules/blossom \
	plugin/modules/swave \
	plugin/modules/cubic \
	plugin/modules/xycloid \
	plugin/modules/fuzzy-logic \
	plugin/modules/tapers \
	plugin/modules/curve-sequencer \
	plugin/modules/curve-sequencer/processor \
	plugin/modules/stage \
	plugin/modules/func \
	plugin/widgets \

SOURCES = $(foreach dir, $(SOURCE_DIRS), $(wildcard $(dir)/*.cpp))

DISTRIBUTABLES += LICENSE.txt svg

FLAGS += $(foreach dir,$(SOURCE_DIRS),-I$(dir))
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

CMAKE_DIR = .cmake-dale

cmake:
	cmake -S plugin -B $(CMAKE_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake --build $(CMAKE_DIR)

test: cmake
	cd $(CMAKE_DIR) && ctest --progress --output-on-failure

clean-cmake:
	rm -rf $(CMAKE_DIR)

clean: clean-cmake

.PHONY: ctest cmake clean-cmake




########################################################################
#
# Analyze and format the code
#
########################################################################

TEST_DIRS = \
	plugin/controls/test \
	plugin/components/test \
	plugin/modules/curve-sequencer/test \

TEST_SOURCES = $(foreach dir, $(TEST_DIRS), $(wildcard $(dir)/*.cpp))

HEADERS = $(foreach dir, $(SOURCE_DIRS), $(wildcard $(dir)/*.h))

format:
	clang-format -i -style=file $(HEADERS) $(SOURCES) $(TEST_SOURCES)

tidy: cmake
	clang-tidy -p=$(CMAKE_DIR) $(SOURCES)

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

