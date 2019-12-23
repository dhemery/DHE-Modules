default: svg
all: svg png

PUML_DIR = $(PWD)/puml
PUML_FILES = $(wildcard $(PUML_DIR)/*.puml)
TMP_DIR = $(PWD)/.tmp-png

PNG_DIR = $(PWD)/png
PNG_FILES := $(patsubst $(PUML_DIR)/%.puml, $(PNG_DIR)/%.png, $(PUML_FILES))
PNG_DPI = 150
$(PNG_DIR)/%.png: $(PUML_DIR)/%.puml
	plantuml -tpng -SDpi=$(PNG_DPI) -o $(PNG_DIR) $^

png: $(PNG_FILES)

SVG_DIR = $(PWD)/svg
SVG_FILES := $(patsubst $(PUML_DIR)/%.puml, $(SVG_DIR)/%.svg, $(PUML_FILES))
$(SVG_DIR)/%.svg: $(PUML_DIR)/%.puml
	plantuml -tsvg -o $(SVG_DIR) $^

svg: $(SVG_FILES)

edit:
	plantuml -gui -o $(TMP_DIR) $(PUML_DIR)

.DEFAULT_GOAL := svg

.PHONY : clean default
clean :
	-rm -rf $(PNG_DIR)
	-rm -rf $(SVG_DIR)
	-rm -rf $(TMP_DIR)
