#!/usr/bin/env bash

# This script uses these environment variables:
#
# VCV_PLUGIN_DIR
#
#   The directory in which to clone and build the plugin.
#
# VCV_PLUGIN_COMMIT
#
#   Which Rack commit to build.
#
# VCV_RACK_DIR
#
#   The directory in which Rack was built.

mkdir -p "${VCV_PLUGIN_DIR}"

git clone https://github.com/dhemery/DHE-Modules "${VCV_PLUGIN_DIR}"
cd "${VCV_PLUGIN_DIR}"
git checkout "${VCV_PLUGIN_COMMIT}"

make clean test dist RACK_DIR="${VCV_RACK_DIR}"
