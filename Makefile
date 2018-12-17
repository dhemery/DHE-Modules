SLUG = DHE-Modules
VERSION = 0.6.4
RACK_DIR ?= ../..

FLAGS += -I./include
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES = $(wildcard src/*.cpp)

DISTRIBUTABLES += LICENSE.txt res

include $(RACK_DIR)/plugin.mk

# Above this line: Standard plugin build stuff
########################################################################
# Below this line: Targets for Dale to build the gui and run Rack

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
	find src include -name *.h -o -name *.cpp | xargs clang-format -i

gui:
	$(MAKE) -C gui clobber all

clobber: fresh
	$(MAKE) -C gui clobber

fresh: clean uninstall

uninstall:
	rm -rf $(DEV_INSTALL_DIR)

.PHONY: clean clobber fresh gui tidy uninstall
