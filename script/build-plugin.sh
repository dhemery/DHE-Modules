#!/usr/bin/env bash

# This script uses these environment variables:
#
# VCV_PLUGIN_DIR
#
#   The directory in which to clone and build the plugin.
#
# VCV_RACK_DIR
#
#   The directory in which Rack was built.

cd "${VCV_PLUGIN_DIR}"

make clean test dist RACK_DIR="${VCV_RACK_DIR}"
