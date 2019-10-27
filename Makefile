RACK_DIR ?= ../..

DHE_SOURCE_DIRS = \
	plugin/* \
	plugin/common/src/* \
	plugin/common/src/*/* \
	plugin/modules/* \

DHE_SOURCE_FILES = $(foreach dir, $(DHE_SOURCE_DIRS), $(wildcard $(dir)/*.cpp))
DHE_INCLUDE_DIRS = plugin/common/include
DHE_INCLUDE_FLAGS = $(foreach dir, $(DHE_INCLUDE_DIRS), -I$(dir))

FLAGS += $(DHE_INCLUDE_FLAGS)
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=
SOURCES = $(DHE_SOURCE_FILES)
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

DHE_APP_DIR ?= /Applications
DHE_RACK_APP = $(DHE_APP_DIR)/Rack.app
DHE_RACK_EXECUTABLE = $(DHE_RACK_APP)/Contents/MacOS/Rack
DHE_RACK_SYSTEM_DIR = $(DHE_RACK_APP)/Contents/Resources
DHE_BUILD_DIR = .build
DHE_STAGING_DIR = .install

configure:
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B $(DHE_BUILD_DIR)

build: configure
	cmake --build $(DHE_BUILD_DIR) --verbose

clean-build:
	rm -rf $(DHE_BUILD_DIR)

test: build
	cd $(DHE_BUILD_DIR) && ctest --progress --output-on-failure

stage: build
	cmake --build $(DHE_BUILD_DIR) --target install

clean-stage:
	rm -rf $(DHE_STAGING_DIR)

run: stage
	$(DHE_RACK_EXECUTABLE) -u $(realpath $(DHE_STAGING_DIR))

debug: stage
	$(DHE_RACK_EXECUTABLE) \
	 	-d \
	 	-s $(realpath $(DHE_RACK_SYSTEM_DIR)) \
	 	-u $(realpath $(DHE_STAGING_DIR))

clean: clean-build clean-stage

.PHONY: build clean-build clean-stage configure debug run stage test




########################################################################
#
# Analyze and format the code
#
########################################################################

DHE_TEST_SOURCE_DIRS = \
	test/* \
	test/*/* \

DHE_INCLUDE_DIRS = \
	include/* \
	include/*/* \

DHE_TEST_SOURCE_FILES = $(foreach dir, $(TEST_SOURCE_DIRS), $(wildcard $(dir)/*.cpp))

DHE_HEADER_FILES = $(foreach dir, $(DHE_INCLUDE_DIRS) $(DHE_SOURCE_DIRS), $(wildcard $(dir)/*.h))

format:
	clang-format -i -style=file $(DHE_HEADER_FILES) $(DHE_SOURCE_FILES) $(DHE_TEST_SOURCE_FILES)

tidy: build
	clang-tidy -p=$(DHE_BUILD_DIR) $(DHE_SOURCE_FILES)

.PHONY: format tidy




########################################################################
#
# Build the SVG files for the GUI
#
########################################################################

gui:
	cd gui && rake install

clobber-gui:
	cd gui && rake clobber

clobber: clobber-gui

.PHONY: gui clobber-gui




########################################################################
#
# Cleanup
#
########################################################################

clobber: clean

.PHONY: clobber
