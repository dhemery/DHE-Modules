FROM ubuntu

LABEL maintainer="dale@dhemery.com"

RUN apt-get update \
  && apt-get install -y software-properties-common \
  && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN apt-get update \
  && apt-get install -y cmake gcc git make tar unzip wget zip \
  && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN apt-get update \
  && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
  && apt-get install -y \
  libasound2-dev \
  libgl1-mesa-dev \
  libglu1-mesa-dev \
  libgtk2.0-dev \
  libudev-dev \
  libxcursor-dev \
  libxinerama-dev\
  libxrandr-dev \
  zlib1g-dev \
  && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN rm -rf /var/lib/apt/lists/*

ENV VCV_BUILD_DIR="/linux-build"
ENV VCV_RACK_BUILD_DIR="${VCV_BUILD_DIR}/Rack"

RUN mkdir -p "${VCV_BUILD_DIR}"
COPY build-rack.sh ${VCV_BUILD_DIR}/
RUN  ${VCV_BUILD_DIR}/build-rack.sh
COPY build-plugin.sh ${VCV_BUILD_DIR}/

ENV VCV_PLUGIN_NAME="DHE-Modules"
ENV VCV_PLUGIN_ARTIFACT_DIR="/linux-dist"

ENV VCV_BUILD_COMMIT=master
CMD ${VCV_BUILD_DIR}/build-plugin.sh
VOLUME ${VCV_PLUGIN_ARTIFACT_DIR}
