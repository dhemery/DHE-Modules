#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o xtrace

echo "MSYSTEM=${MSYSTEM}"
export PLUGIN_NAME=DHE-Modules
export PLUGIN_DIR="${APPVEYOR_BUILD_FOLDER}"
export SDK_DIR=/c/tmp/Rack-SDK
export SDK_VERSION=0.6.0

env | sort

cd /c/tmp
curl -o rack-sdk.zip https://vcvrack.com/downloads/Rack-SDK-${SDK_VERSION}.zip
unzip rack-sdk.zip

cd "${PLUGIN_DIR}"

make clean test dist RACK_DIR="${SDK_DIR}"


