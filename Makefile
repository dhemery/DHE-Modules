SLUG = DHE-Modules
VERSION = 0.6.5
RACK_DIR ?= ../..

FLAGS += -I./include -MJ $@.json
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*.cpp)

DISTRIBUTABLES += LICENSE.txt svg

include $(RACK_DIR)/plugin.mk

# Above this line: Standard plugin build stuff
########################################################################
# Below this line: Targets for Dale to build the gui and run Rack

TEST_SOURCES =  $(wildcard test/*.cpp)
TEST_OBJECTS := $(patsubst %, build/%.o, $(TEST_SOURCES))
TESTFLAGS += -Igoogletest/googletest/include/ -Igoogletest/googlemock/include/

$(TEST_OBJECTS): FLAGS += $(TESTFLAGS)

TEST_RUNNER = build/dhe-module-tests

$(TEST_RUNNER): $(TEST_OBJECTS)
	$(CXX) -o $@ $^ -stdlib=libc++ -Lgoogletest/lib -lgmock_main -lgtest -lgmock

test-runner: $(TEST_RUNNER)

test: $(TEST_RUNNER)
	$<

DEV_INSTALL_DIR = .dev
DEV_PLUGIN_DIR = $(DEV_INSTALL_DIR)/plugins

$(DEV_INSTALL_DIR) $(DEV_PLUGIN_DIR):
	mkdir -p $@

$(DEV_PLUGIN_DIR)/Fundamental: $(DEV_PLUGIN_DIR)
	cp -r $(RACK_USER_DIR)/plugins/Fundamental $(DEV_PLUGIN_DIR)

dev: dist $(DEV_PLUGIN_DIR) $(DEV_PLUGIN_DIR)/Fundamental
	cp dist/$(SLUG)-$(VERSION)-$(ARCH).zip $(DEV_PLUGIN_DIR)

debug: dev
	/Applications/Rack.app/Contents/MacOS/Rack -d -g /Applications/Rack.app/Contents/Resources -l $(realpath $(DEV_INSTALL_DIR))

run: dev
	/Applications/Rack.app/Contents/MacOS/Rack -g /Applications/Rack.app/Contents/Resources -l $(realpath $(DEV_INSTALL_DIR))

tidy:
	find src include -name *.h -o -name *.cpp | xargs clang-format -i -style=file

COMPILE_DB_JSONS := $(patsubst %, %.json, $(OBJECTS) $(TEST_OBJECTS) )

$(COMPILE_DB_JSONS): $(OBJECTS) $(TEST_OBJECTS)

compile_commands.json: $(COMPILE_DB_JSONS)
	sed -e '1s/^/[/' -e '$$s/,$$/]/' $^ | json_pp > $@

compiledb: compile_commands.json

gui:
	cd gui && rake install

clobber: clean uninstall
	cd gui && rake clobber

fresh: clean all

uninstall:
	rm -rf $(DEV_INSTALL_DIR)

.PHONY: clean clobber fresh gui tidy uninstall

