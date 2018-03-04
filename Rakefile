require 'rake/clean'
require 'pathname'

desc 'Build the site'
task :svg do
  sh 'bundle', 'exec', 'jekyll', 'b', '--trace'
end

module_names = ['booster-stage', 'stage', 'swave', 'upstage']

panel_deps = []

module_names.each do |module_name|
  res_dir = Pathname("res/#{module_name}/").expand_path
  source_file = Pathname("svg-builder/#{module_name}/fonted/panel.svg")
  fonted_file = res_dir / 'fonted/panel.svg'
  final_file = res_dir / 'panel.svg'

  file fonted_file => source_file do
    Rake::Task[:svg].invoke
  end

  file final_file => fonted_file do
    sh '/Applications/Inkscape.app/Contents/Resources/script', '--export-text-to-path', "--export-plain-svg=#{final_file}", fonted_file.to_s
  end

  task panels: final_file
end

task default: :panels

CLEAN.include 'res'
