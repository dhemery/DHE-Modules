RACK_DIR ?= ../..

FLAGS += -Isrc
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=
SOURCES = $(wildcard src/*.cpp)
DISTRIBUTABLES += LICENSE.txt presets svg

include $(RACK_DIR)/plugin.mk




########################################################################
#
# Build and run the tests
#
########################################################################

TEST_SOURCES = $(wildcard \
 						tests/*.cpp \
 						tests/*/*.cpp \
 						tests/*/*/*.cpp \
 						)

TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))

TESTFLAGS += -Itest/ -Igoogletest/googletest/include/ -Igoogletest/googlemock/include/
TESTLDFLAGS += -Lgoogletest/lib -lgmock_main -lgtest -lgmock

ifdef ARCH_LIN
	TESTLDFLAGS += -lpthread
endif

$(TEST_OBJECTS): FLAGS += $(TESTFLAGS)

TEST_RUNNER = build/test-runner

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^ $(TESTLDFLAGS)

test: $(TEST_RUNNER)
	$<

googletest:
	cd googletest && cmake . && cmake --build . && rm -rf googletest/generated

.PHONY: test googletest




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

HEADER_FILES = $(wildcard \
					src/*.h \
					src/*/*.h \
					src/*/*/*.h \
					)

format:
	clang-format -i -style=file $(HEADER_FILES) $(SOURCE_FILES) $(TEST_SOURCE_FILES)

tidy: build
	clang-tidy -header-filter=.* -p=$(BUILD_DIRNAME) $(SOURCE_FILES)

.PHONY: format tidy
