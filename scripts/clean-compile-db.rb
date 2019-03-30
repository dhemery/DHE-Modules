#!/usr/bin/env ruby
require 'json'

filename = 'compile_commands.json'

json = File.read(filename)
parsed = JSON.parse(STDIN.read)
sorted = parsed.map { |f| f.sort.to_h }
generated = JSON.pretty_generate(sorted)
puts generated
