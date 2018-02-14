#!/usr/bin/env bash

cd "$(dirname "$0")"

. ./build-rack.sh

export VCV_PLUGIN_BUILD_DIR="${VCV_RACK_PLUGINS_DIR}/${VCV_PLUGIN_NAME}"

env | sort

git clone https://github.com/dhemery/DHE-Modules "${VCV_PLUGIN_BUILD_DIR}"
cd "${VCV_PLUGIN_BUILD_DIR}"
git checkout "${VCV_BUILD_COMMIT}"

make clean test dist

mkdir -p "${VCV_PLUGIN_ARTIFACT_DIR}"
cp dist/*.zip "${VCV_PLUGIN_ARTIFACT_DIR}/"
