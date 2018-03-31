RACK_SDK_VERSION ?= 0.6.0
RACK_REPOSITORY := rack
RACK_ENVIRONMENT_TAG := env
RACK_ENVIRONMENT_IMAGE := $(RACK_REPOSITORY):$(RACK_ENVIRONMENT_TAG)
RACK_SDK_TAG := v$(RACK_SDK_VERSION)
RACK_SDK_IMAGE := $(RACK_REPOSITORY):$(RACK_SDK_TAG)

all: racksdk

run: racksdk
	docker run -it $(RACK_SDK_IMAGE)

racksdk: rackenv
	docker image build \
		--build-arg RACK_SDK_VERSION=$(RACK_SDK_VERSION) \
		-t $(RACK_SDK_IMAGE) \
		sdk/

rackenv:
	docker image build -t $(RACK_ENVIRONMENT_IMAGE) $(RACK_ENVIRONMENT_TAG)/

clobber: clean
	docker rmi $(RACK_SDK_IMAGE) $(RACK_ENVIRONMENT_IMAGE) || true

clean:

.PHONY: all build clean clobber


