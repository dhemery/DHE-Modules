DHE_BUILD_DIR = .build
DHE_STAGING_DIR = .stage
DHE_RACK_STAGING_DIR = rack-user-dir

DHE_CMAKE_OPTIONS += -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
DHE_CMAKE_OPTIONS += -DCMAKE_INSTALL_PREFIX=$(DHE_STAGING_DIR)
DHE_CMAKE_OPTIONS += -DRACK_STAGING_DIR=$(DHE_RACK_STAGING_DIR)
DHE_CMAKE_OPTIONS += -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON

DHE_CMAKE_BUILD_OPTIONS +=

DHE_CTEST_OPTIONS += --progress
DHE_CTEST_OPTIONS += --output-on-failure

DHE_APP_DIR ?= /Applications
DHE_RACK_APP = $(DHE_APP_DIR)/Rack.app
DHE_RACK_EXECUTABLE = $(DHE_RACK_APP)/Contents/MacOS/Rack
DHE_RACK_SYSTEM_DIR = $(DHE_RACK_APP)/Contents/Resources

$(DHE_BUILD_DIR):
	mkdir $@

config: $(DHE_BUILD_DIR)
	cd $(DHE_BUILD_DIR) && cmake $(DHE_CMAKE_OPTIONS) ..

build: config
	cmake --build $(DHE_BUILD_DIR) $(DHE_CMAKE_BUILD_OPTIONS)

clean-build:
	rm -rf $(DHE_BUILD_DIR)

test: build
	cd $(DHE_BUILD_DIR) && ctest $(DHE_CTEST_OPTIONS)

stage: build
	cmake --build $(DHE_BUILD_DIR) --target install

clean-stage:
	rm -rf $(DHE_STAGING_DIR)

run: stage
	$(DHE_RACK_EXECUTABLE) \
		-u $(realpath $(DHE_STAGING_DIR))/$(DHE_RACK_STAGING_DIR)

debug: stage
	$(DHE_RACK_EXECUTABLE) \
	 	-d \
	 	-u $(realpath $(DHE_STAGING_DIR))/$(DHE_RACK_STAGING_DIR) \
	 	-s $(realpath $(DHE_RACK_SYSTEM_DIR))

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

