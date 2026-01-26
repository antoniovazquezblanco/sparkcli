# Include directories
inc_dirs = include_directories('.')

# Source files for SparkCLI library
sparkcli_sources = files([
  'utils/env.c',
  'log.c',
])

# Library target
sparkcli_lib = static_library(
  'sparkcli',
  sources: sparkcli_sources,
  include_directories: inc_dirs,
  install: true
)

# Install headers
install_headers(
  'sparkcli.h',
  'log.h',
  subdir: 'sparkcli'
)

# Create dependency object for use by other projects
sparkcli_dep = declare_dependency(
  include_directories: inc_dirs,
  link_with: sparkcli_lib
)
