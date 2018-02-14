#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o xtrace

export VCV_RACK_BUILD_DIR="${VCV_BUILD_DIR}/Rack"
export VCV_RACK_PLUGINS_DIR="${VCV_RACK_BUILD_DIR}/plugins"

git clone https://github.com/VCVRack/Rack.git "${VCV_RACK_BUILD_DIR}" || true
cd "${VCV_RACK_BUILD_DIR}"
git pull
git submodule update --init --recursive
make dep > /dev/null
make
