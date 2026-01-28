# Include directories
inc_dirs = include_directories('.')

# Source files for SparkCLI library
sparkcli_sources = files([
  'utils/env.c',
  'platform/console_common.c',
  'color.c',
  'log.c',
  'table.c',
])

# Add platform-specific console source
if host_machine.system() == 'windows'
  sparkcli_sources += files('platform/console_windows.c')
else
  sparkcli_sources += files('platform/console_other.c')
endif

# Library target
sparkcli_lib = static_library(
  'sparkcli',
  sources: sparkcli_sources,
  include_directories: inc_dirs,
  install: true
)

# Install headers
install_headers(
  'color.h',
  'log.h',
  'table.h',
  'sparkcli.h',
  subdir: 'sparkcli'
)

# Create dependency object for use by other projects
sparkcli_dep = declare_dependency(
  include_directories: inc_dirs,
  link_with: sparkcli_lib
)
