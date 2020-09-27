require 'rake/clean'

desc 'Build the site'
task :build do
  sh 'bundle exec jekyll b --trace'
end

desc 'Serve the site'
task :serve do
  sh %{bundle exec jekyll s --livereload --baseurl ''}
end

task all: [:build]

task default: :all

CLEAN.include '_site'
