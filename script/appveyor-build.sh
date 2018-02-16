#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o xtrace

echo "MSYSTEM=${MSYSTEM}"
export VCV_PLUGIN_NAME=DHE-Modules
export VCV_PLUGIN_DIR="${APPVEYOR_BUILD_FOLDER}"
export VCV_RACK_DIR=/c/tmp/Rack
export VCV_RACK_COMMIT=master
export VCV_HOME_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

env | sort

. "${VCV_HOME_DIR}/build-rack.sh"
. "${VCV_HOME_DIR}/build-plugin.sh"
