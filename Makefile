include makefiles/file-definitions.mk

FLAGS += $(DHE_INCLUDE_FLAGS)
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=
SOURCES = $(DHE_SOURCE_FILES)
DISTRIBUTABLES += $(DHE_DISTRIBUTABLES)

RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk




########################################################################
#
# Build and test the plugin via CMake and CTest
#
########################################################################

DHE_STAGING_DIRNAME = .stage
DHE_STAGING_DIRPATH = $(realpath .)/$(DHE_STAGING_DIRNAME)

DHE_RACK_USER_DIRNAME = rack-user-dir

DHE_CMAKE_OPTIONS += -DCMAKE_INSTALL_PREFIX=$(DHE_STAGING_DIRPATH)
DHE_CMAKE_OPTIONS += -DRACK_STAGING_DIR=$(DHE_RACK_USER_DIRNAME)
DHE_CMAKE_OPTIONS += -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

DHE_CMAKE_BUILD_OPTIONS +=

DHE_CTEST_OPTIONS += --progress
DHE_CTEST_OPTIONS += --output-on-failure

include makefiles/cmake-targets.mk




########################################################################
#
# Stage the plugin and run Rack
#
########################################################################

DHE_APP_DIRPATH ?= /Applications
DHE_RACK_APP_DIRPATH = $(DHE_APP_DIRPATH)/Rack.app
DHE_RACK_EXECUTABLE_PATH = $(DHE_RACK_APP_DIRPATH)/Contents/MacOS/Rack
DHE_RACK_SYSTEM_DIRPATH = $(DHE_RACK_APP_DIRPATH)/Contents/Resources
DHE_RACK_USER_DIRPATH = $(DHE_STAGING_DIRPATH)/$(DHE_RACK_USER_DIRNAME)

stage: build
	cmake --build $(DHE_BUILD_DIRNAME) --target install  $(DHE_CMAKE_BUILD_OPTIONS)

clean-stage:
	rm -rf $(DHE_STAGING_DIRNAME)

run: stage
	$(DHE_RACK_EXECUTABLE_PATH) -u $(DHE_RACK_USER_DIRPATH)

clean: clean-stage

.PHONY: debug run clean-stage




########################################################################
#
# Analyze and format the code
#
########################################################################

DHE_TEST_SOURCE_FILES = $(wildcard tests/*.cpp tests/*/*.cpp tests/*/*/*.cpp)

DHE_HEADER_FILES = $(wildcard src/*.h src/*/*.h src/*/*/*.h)

format:
	clang-format -i -style=file $(DHE_HEADER_FILES) $(DHE_SOURCE_FILES) $(DHE_TEST_SOURCE_FILES)

tidy: build
	clang-tidy -p=$(DHE_BUILD_DIRNAME) $(DHE_SOURCE_FILES)

.PHONY: format tidy
