require 'rake/clean'

desc 'Build the site'
task :svg do
  sh 'bundle', 'exec', 'jekyll', 'b', '--trace'
end

task default: :svg

CLEAN.include 'res'

# /Applications/Inkscape.app/Contents/Resources/script -T -l `pwd`/output.svg `pwd`/input.svg
