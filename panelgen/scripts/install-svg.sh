#!/usr/bin/env zsh

if [[ "$#" -lt 2 ]]; then
    echo "usage: $(basename "${0}") from-file to-file [inkscape-option ...]"
    exit 1
fi

from="$1"
shift
to="$1"
shift

declare -a convert_texts_to_paths=(
    '--actions=select-by-element:text;object-to-path'
    '--export-filename=-'
    '--export-plain-svg'
)

if grep --silent '<text' "${from}"; then
    inkscape "${convert_texts_to_paths[@]}" "${@}" "${from}" \
    | sed -E 's/ id="[^"]*"//g' \
    | sed -E 's/ aria-label="[^"]*"//g' \
    | xmllint --format - > "${to}"
else
    xmllint --format "${from}" --output "${to}"
fi
