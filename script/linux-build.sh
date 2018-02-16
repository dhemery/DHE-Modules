#!/usr/bin/env bash
set -o errexit
set -o nounset
set -o xtrace

export VCV_HOME_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

env | sort

. "${VCV_HOME_DIR}/build-rack.sh"

git clone https://github.com/dhemery/DHE-Modules "${VCV_PLUGIN_DIR}"
cd "${VCV_PLUGIN_DIR}"
git checkout "${VCV_PLUGIN_COMMIT}"

. "${VCV_HOME_DIR}/build-plugin.sh"

mkdir -p ${VCV_INSTALL_DIR}
cp ${VCV_PLUGIN_DIR}/dist/*.zip ${VCV_INSTALL_DIR}
