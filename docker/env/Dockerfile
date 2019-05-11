FROM ubuntu:xenial
ADD https://cmake.org/files/v3.10/cmake-3.10.3-Linux-x86_64.sh /cmake-3.10.3-Linux-x86_64.sh
RUN mkdir /opt/cmake \
  && sh /cmake-3.10.3-Linux-x86_64.sh --prefix=/opt/cmake --skip-license \
  && ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake \
  && cmake --version \
  && apt-get update \
  && apt-get install -y --no-install-recommends software-properties-common \
  && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
  && apt-get update \
  && apt-get install -y --no-install-recommends \
  autoconf \
  automake \
  curl \
  g++ \
  git \
  jq \
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
