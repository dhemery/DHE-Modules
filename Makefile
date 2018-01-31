SLUG = DHE-Modules
VERSION = 0.5.0-beta.4

FLAGS += -I./src
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES += $(wildcard src/**/*.cpp)

DISTRIBUTABLES +=  $(wildcard LICENSE*) res

include ../../plugin.mk

# TODO: Delete to the end of the file once VCV Rack makefile handles it
DISTRIBUTABLES += $(TARGET)
dist: all
	rm -rf dist
	mkdir -p dist/$(SLUG)
	cp -R $(DISTRIBUTABLES) dist/$(SLUG)/
	cd dist && zip -5 -r $(SLUG)-$(VERSION)-$(ARCH).zip $(SLUG)
