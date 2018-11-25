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

RACK_APP = /Applications/Rack.app

RACK_PLUGINS_DIR = $(RACK_USER_DIR)/plugins
MODULE_INSTALL_DIR = $(RACK_PLUGINS_DIR)/$(SLUG)
MODULE_INSTALL_ZIP = $(RACK_PLUGINS_DIR)/$(SLUG)-$(VERSION)-$(ARCH).zip

run: install
	open $(RACK_APP)

uninstall:
	rm -rf $(MODULE_INSTALL_DIR) $(MODULE_INSTALL_ZIP)

.PHONY: gui
gui:
	$(MAKE) -C gui clobber all


.PHONY: clobber
clobber: clean uninstall
	$(MAKE) -C gui clobber
