DHE_BUILD_DIRNAME = .build

DHE_STAGING_DIRNAME = .stage
DHE_STAGING_DIRPATH = $(realpath .)/$(DHE_STAGING_DIRNAME)

DHE_RACK_USER_DIRNAME = rack-user-dir
DHE_RACK_USER_DIRPATH = $(DHE_STAGING_DIRPATH)/$(DHE_RACK_USER_DIRNAME)

DHE_CMAKE_OPTIONS += -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
DHE_CMAKE_OPTIONS += -DCMAKE_INSTALL_PREFIX=$(DHE_STAGING_DIRPATH)
DHE_CMAKE_OPTIONS += -DRACK_STAGING_DIR=$(DHE_RACK_USER_DIRNAME)
DHE_CMAKE_OPTIONS += -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON

DHE_CMAKE_BUILD_OPTIONS +=

DHE_CTEST_OPTIONS += --progress
DHE_CTEST_OPTIONS += --output-on-failure

DHE_APP_DIRPATH ?= /Applications
DHE_RACK_APP_DIRPATH = $(DHE_APP_DIRPATH)/Rack.app
DHE_RACK_EXECUTABLE_PATH = $(DHE_RACK_APP_DIRPATH)/Contents/MacOS/Rack
DHE_RACK_SYSTEM_DIRPATH = $(DHE_RACK_APP_DIRPATH)/Contents/Resources

$(DHE_BUILD_DIRNAME):
	mkdir $@

config: $(DHE_BUILD_DIRNAME)
	cd $(DHE_BUILD_DIRNAME) && cmake $(DHE_CMAKE_OPTIONS) ..

build: config
	cmake --build $(DHE_BUILD_DIRNAME) $(DHE_CMAKE_BUILD_OPTIONS)

clean-build:
	rm -rf $(DHE_BUILD_DIRNAME)

test: build
	cd $(DHE_BUILD_DIRNAME) && ctest $(DHE_CTEST_OPTIONS)

stage: build
	cmake --build $(DHE_BUILD_DIRNAME) --target install  $(DHE_CMAKE_BUILD_OPTIONS)

clean-stage:
	rm -rf $(DHE_STAGING_DIRNAME)

run: stage
	$(DHE_RACK_EXECUTABLE_PATH) -u $(DHE_RACK_USER_DIRPATH)

debug: stage
	$(DHE_RACK_EXECUTABLE_PATH) -d -u $(DHE_RACK_USER_DIRPATH) -s $(DHE_RACK_SYSTEM_DIRPATH)

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
	clang-tidy -p=$(DHE_BUILD_DIRNAME) $(DHE_SOURCE_FILES)

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

