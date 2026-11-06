# Include directories
inc_dirs = include_directories('.')

# Source files for SparkCLI library
sparkcli_sources = files([
  'utils/env.c',
  'platform/console_common.c',
  'color.c',
  'session.c',
  'input.c',
  'log.c',
  'table.c',
  'progress.c',
  'tree.c',
  'hexdump.c'
])

# Add platform-specific console source
if host_machine.system() == 'windows'
  sparkcli_sources += files('platform/console_windows.c')
else
  sparkcli_sources += files('platform/console_other.c')
endif

# A consumer that statically links SparkCLI has no use for the installed
# artifacts, so only install when SparkCLI is the top level project.
install_sparkcli = not meson.is_subproject()

# Library target
sparkcli_lib = static_library(
  'sparkcli',
  sources: sparkcli_sources,
  include_directories: inc_dirs,
  install: install_sparkcli
)

# Install headers
if install_sparkcli
  install_headers(
    'color.h',
    'session.h',
    'input.h',
    'log.h',
    'table.h',
    'progress.h',
    'tree.h',
    'hexdump.h',
    'sparkcli.h',
    subdir: 'sparkcli'
  )
endif

# Create dependency object for use by other projects
sparkcli_dep = declare_dependency(
  include_directories: inc_dirs,
  link_with: sparkcli_lib
)
