#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o xtrace

env | sort

cd "$(dirname "$0")"
. ./build-rack.sh

git clone https://github.com/dhemery/DHE-Modules "${VCV_PLUGIN_DIR}"
cd "${VCV_PLUGIN_DIR}"
git checkout "${VCV_PLUGIN_COMMIT}"

cd "$(dirname "$0")"
. ./build-plugin.sh

mkdir -p ${VCV_INSTALL_DIR}
cp ${VCV_PLUGIN_DIR}/dist/*.zip ${VCV_INSTALL_DIR}
