require 'rake/clean'
require 'pathname'

booster_stage_dir = Pathname('res/booster-stage/').expand_path
booster_stage_fonted_panel = booster_stage_dir / 'fonted/panel.svg'
booster_stage_panel = booster_stage_dir / 'panel.svg'

swave_dir = Pathname('res/swave/').expand_path
swave_fonted_panel = swave_dir / 'fonted/panel.svg'
swave_panel = swave_dir / 'panel.svg'


desc 'Build the site'
task :svg do
  sh 'bundle', 'exec', 'jekyll', 'b', '--trace'
end

task panels: [:svg] do
  sh '/Applications/Inkscape.app/Contents/Resources/script', '-T', '-l', booster_stage_panel.to_s, booster_stage_fonted_panel.to_s
  sh '/Applications/Inkscape.app/Contents/Resources/script', '-T', '-l', swave_panel.to_s, swave_fonted_panel.to_s
end

task default: :panels

CLEAN.include 'res'

# /Applications/Inkscape.app/Contents/Resources/script -T -l `pwd`/output.svg `pwd`/input.svg
