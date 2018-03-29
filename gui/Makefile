BUILD_DIR = _build

SOURCES := $(wildcard panels/*.svg)
JEKYLL_FILES := _config.yaml Gemfile.lock
JEKYLL_FILES += $(wildcard _layouts/* _includes/*)

# Building any file will build all files, so I arbitrarily picked this one
$(BUILD_DIR)/panels/booster-stage.svg: $(SOURCES) $(JEKYLL_FILES)
	bundle exec jekyll build

clean:
	rm -rf $(BUILD_DIR)
