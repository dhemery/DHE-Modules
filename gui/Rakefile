class Float
  def to_s
    '%.3f' % self
  end
end

require 'rake/clean'
require 'pathname'
require_relative 'lib/module_factory'
require_relative 'lib/svg_file'

def install_svg(from, to, *options)
  from = from.expand_path.to_s
  to = to.expand_path.to_s
  sh './scripts/install-svg.sh', from, to, *options
end

def install_faceplate(from, to)
  install_svg from, to, '--export-id=faceplate', '--export-id-only'
end

modules_dir = Pathname('modules')
controls_build_dir = Pathname('_controls')
faceplates_build_dir = Pathname('_faceplates')
svg_install_dir = Pathname('../svg')
image_install_dir = Pathname('../images')

directory controls_build_dir
directory faceplates_build_dir
directory svg_install_dir
directory image_install_dir

modules = FileList.new(modules_dir / '*/*.rb').map { |file| ModuleFactory.new(file).build }

modules.each do |mod|
  module_source_file = mod.source_file
  file module_source_file

  module_svg_path = mod.slug.sub_ext('.svg')


  faceplate_build_file = faceplates_build_dir / module_svg_path
  faceplate_build_dir = faceplate_build_file.dirname
  directory faceplate_build_dir
  file faceplate_build_file => [faceplate_build_dir, module_source_file] do
    ModuleSvgFile.new(path: faceplate_build_file, mod: mod).write
  end
  task build: faceplate_build_file

  helpers = FileList.new(mod.source_file.parent / 'helpers/*.rb')
  helpers.each do |helper|
    file helper
    file faceplate_build_dir => helper
  end unless helpers.empty?

  faceplate_install_file = svg_install_dir / module_svg_path
  file faceplate_install_file => [svg_install_dir, faceplate_build_file] do
    install_faceplate(faceplate_build_file, faceplate_install_file)
  end
  task install: faceplate_install_file

  image_install_file = image_install_dir / module_svg_path
  file image_install_file => [image_install_dir, faceplate_build_file] do
    install_svg(faceplate_build_file, image_install_file)
  end
  task install: image_install_file
end

controls_by_control_svg_path = {}

modules.each do |mod|
  mod.controls.each do |control|
    control.frames.map do |frame|
      control_svg_path = mod.dir / frame.slug.sub_ext('.svg')
      controls_by_control_svg_path[control_svg_path] = { content: frame, source_file: mod.source_file }
    end
  end
end

controls_by_control_svg_path.each do |control_svg_path, control|
  control_source_file = control[:source_file]
  control_content = control[:content]

  control_build_file = controls_build_dir / control_svg_path
  control_build_dir = control_build_file.parent

  directory control_build_dir
  file control_build_file => [control_build_dir, control_source_file] do
    ControlSvgFile.new(path: control_build_file, content: control_content).write
  end
  task build: control_build_file

  control_install_file = svg_install_dir / control_svg_path
  control_install_dir = control_install_file.parent

  directory control_install_dir
  file control_install_file => [control_install_dir, control_build_file] do
    install_svg(control_build_file, control_install_file)
  end
  task install: control_install_file
end

desc 'Build SVG files'
task :build

desc 'Install SVG files'
task install: [:build]

desc 'Remove and rebuild SVG files'
task fresh: [:clean, :default]

desc 'Remove, rebuild, and reinstall SVG files'
task reinstall: [:clobber, :install]

task default: [:build]

CLEAN.include controls_build_dir, faceplates_build_dir
CLOBBER.include image_install_dir, svg_install_dir
