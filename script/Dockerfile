FROM ubuntu

LABEL maintainer="dale@dhemery.com"

RUN apt-get update \
  && apt-get install -y --no-install-recommends software-properties-common \
  && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
  && apt-get update \
  && apt-get install -y --no-install-recommends \
  autoconf \
  automake \
  cmake \
  curl \
  g++ \
  git \
  libasound2-dev \
  libgl1-mesa-dev \
  libglu1-mesa-dev \
  libgtk2.0-dev \
  libtool \
  libudev-dev \
  libxcursor-dev \
  libxinerama-dev\
  libxrandr-dev \
  make \
  tar \
  unzip \
  wget \
  zip \
  zlib1g-dev \
  && add-apt-repository -yr ppa:ubuntu-toolchain-r/test \
  && apt-get autoremove --purge \
  && apt-get clean

ENV VCV_BUILD_DIR=/plugin \
    VCV_INSTALL_DIR=/out \
    VCV_PLUGIN_NAME=DHE-Modules

WORKDIR ${VCV_BUILD_DIR}

COPY build-rack.sh .

ENV VCV_RACK_COMMIT=master \
    VCV_RACK_DIR=${VCV_BUILD_DIR}/Rack

RUN mkdir -p "${VCV_RACK_DIR}" \
    && git clone -n https://github.com/VCVRack/Rack.git "${VCV_RACK_DIR}" || true \
    && cd "${VCV_RACK_DIR}" \
    && git checkout ${VCV_RACK_COMMIT} \
    && git pull \
    && git submodule update --init --recursive \
    && cd "${VCV_RACK_DIR}" && make dep > /dev/null

RUN ./build-rack.sh

COPY build-plugin.sh .

ENV VCV_PLUGIN_COMMIT=master \
    VCV_PLUGIN_DIR=${VCV_BUILD_DIR}/${VCV_PLUGIN_NAME}

CMD ./linux-build.sh

VOLUME ${VCV_INSTALL_DIR}
