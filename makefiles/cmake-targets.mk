DHE_BUILD_DIRNAME = .build

DHE_CMAKE_OPTIONS +=
DHE_CMAKE_BUILD_OPTIONS +=
DHE_CTEST_OPTIONS +=

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

clean: clean-build

.PHONY: build clean-build config test
