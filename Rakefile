require 'rake/clean'
require 'pathname'

desc 'Build the site'
task :svg do
  sh 'bundle', 'exec', 'jekyll', 'b', '--trace'
end

module_names = ['booster-stage', 'hostage', 'stage', 'swave', 'upstage']

panel_deps = []

module_names.each do |module_name|
  source_file = Pathname("svg-builder/#{module_name}/-panel.svg").expand_path.to_s
  intermediate_file = Pathname("res/#{module_name}/-panel.svg").expand_path.to_s
  panel_file = Pathname("res/#{module_name}/panel.svg").expand_path.to_s

  file intermediate_file => source_file do
    Rake::Task[:svg].invoke
  end

  file panel_file => intermediate_file do
    sh '/Applications/Inkscape.app/Contents/Resources/script', '--export-text-to-path', "--export-plain-svg=#{panel_file}", intermediate_file
  end

  task panels: panel_file
end

task all: [:svg, :panels]

task default: :all

CLEAN.include 'res'
