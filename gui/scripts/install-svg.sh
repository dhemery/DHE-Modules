#!/usr/bin/env zsh

if [[ "$#" -lt 2 ]]; then
  echo "usage: $(basename "${0}") from-file to-file [inkscape-option ...]"
  exit 1
fi

from="$1"
shift
to="$1"
shift
inkscape_options="$*"

mkdir -p "$(dirname ${to})"

if grep --silent '<text' "${from}"; then
  inkscape '--actions=select-by-element:text;object-to-path' --export-filename=- --export-plain-svg "${@}" "${from}" | xmllint --format - > "${to}"
else
  xmllint --format "${from}" --output "${to}"
fi
