RACK_SDK_VERSION ?= 0.6.1
RACK_REPOSITORY := rack
RACK_ENVIRONMENT_TAG := env
RACK_ENVIRONMENT_IMAGE := $(RACK_REPOSITORY):$(RACK_ENVIRONMENT_TAG)
RACK_SDK_TAG := v$(RACK_SDK_VERSION)
RACK_SDK_IMAGE := $(RACK_REPOSITORY):$(RACK_SDK_TAG)

all: racksdk

run: racksdk
	docker run -it $(RACK_SDK_IMAGE)

racksdk: rackenv
	docker image inspect $(RACK_SDK_IMAGE) --format "{{.ID}}" \
	|| docker image build -t $(RACK_SDK_IMAGE) sdk/ \
		--build-arg RACK_SDK_VERSION=$(RACK_SDK_VERSION)

rackenv:
	docker image inspect $(RACK_ENVIRONMENT_IMAGE) --format "{{.ID}}" \
	|| docker image build -t $(RACK_ENVIRONMENT_IMAGE) $(RACK_ENVIRONMENT_TAG)/

cleansdk:
	docker image rm $(RACK_SDK_IMAGE)

cleanenv:
	docker image rm $(RACK_ENVIRONMENT_IMAGE)

clean: cleansdk cleanenv

.PHONY: all build clean cleanenv cleansdk 


