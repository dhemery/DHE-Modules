#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o xtrace

echo "MSYSTEM=${MSYSTEM}"
export VCV_PLUGIN_NAME=DHE-Modules
export VCV_PLUGIN_DIR="${APPVEYOR_BUILD_FOLDER}"
export VCV_RACK_DIR="${VCV_PLUGIN_DIR}/../Rack"
export VCV_RACK_COMMIT=master

env | sort

cd "$(dirname "$0")"
. ./build-rack.sh

cd "$(dirname "$0")"
. ./build-plugin.sh
