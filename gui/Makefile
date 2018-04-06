RESOURCE_DIR = res
IMAGE_DIR = images

JEKYLL_FILES := _config.yaml Gemfile.lock
JEKYLL_FILES += $(wildcard _layouts/* _includes/*)
SOURCE_DIR = panels
SOURCES := $(wildcard $(SOURCE_DIR)/*.svg)

BUILD_DIR = _build

IMAGE_BUILD_DIR = $(BUILD_DIR)/images
IMAGE_BUILD_FILES = $(patsubst $(SOURCE_DIR)/%.svg,$(IMAGE_BUILD_DIR)/%.svg,$(SOURCES))
IMAGES = $(patsubst $(SOURCE_DIR)/%.svg,$(IMAGE_DIR)/%.svg,$(SOURCES))

PANEL_BUILD_DIR = $(BUILD_DIR)/panels
PANEL_BUILD_FILES = $(patsubst $(SOURCE_DIR)/%.svg,$(PANEL_BUILD_DIR)/%.svg,$(SOURCES))
PANELS = $(patsubst $(SOURCE_DIR)/%.svg,$(RESOURCE_DIR)/%/panel.svg,$(SOURCES))

CONTROL_BUILD_DIR = $(BUILD_DIR)/controls
PORT_BUILD_FILES = $(patsubst $(SOURCE_DIR)/%.svg,$(CONTROL_BUILD_DIR)/%/port.svg,$(SOURCES))
PORTS = $(patsubst $(SOURCE_DIR)/%.svg,$(RESOURCE_DIR)/%/port.svg,$(SOURCES))

RESOURCES = $(PANELS) $(PORTS)

all: $(IMAGES) $(RESOURCES)

info:
	@echo "Sources: $(SOURCES)"
	@echo "Port build files: $(PORT_BUILD_FILES)"
	@echo "Ports: $(PORTS)"

$(IMAGE_BUILD_FILES) $(PANEL_BUILD_FILES) $(PORT_BUILD_FILES): $(SOURCES) $(JEKYLL_FILES)
	jekyll build --trace

$(IMAGE_DIR)/%.svg: $(IMAGE_BUILD_DIR)/%.svg
	mkdir -p $(dir $@)
	/Applications/Inkscape.app/Contents/Resources/script --export-text-to-path --export-plain-svg=$(abspath $@) $(abspath $<)

$(RESOURCE_DIR)/%/panel.svg: $(PANEL_BUILD_DIR)/%.svg
	mkdir -p $(dir $@)
	/Applications/Inkscape.app/Contents/Resources/script --export-text-to-path --export-plain-svg=$(abspath $@) $(abspath $<)

$(RESOURCE_DIR)/%/port.svg: $(CONTROL_BUILD_DIR)/%/port.svg
	rsync -a $(dir $<) $(dir $@)

clean:
	rm -rf $(BUILD_DIR)

clobber: clean
	rm -rf $(RESOURCE_DIR) $(IMAGE_DIR)

