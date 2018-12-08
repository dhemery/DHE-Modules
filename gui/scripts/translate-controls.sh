#!/usr/bin/env bash

source_dir=${1%/}
dest_dir=${2%/}

for source in $(grep -irl '<text' ${source_dir})
do
    dest_file=$(pwd)/${dest_dir}/${source#${source_dir}/}
    source_file=$(pwd)/${source}
	/Applications/Inkscape.app/Contents/Resources/script --export-text-to-path --export-plain-svg="${dest_file}" "${source_file}"
done
