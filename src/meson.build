# The version parts come from the script so the parsing lives in one place,
# tested on its own, rather than being re-done in Meson string operations.
version_parts = configuration_data()
version_values = {}
foreach line : run_command(meson.project_source_root() / 'meson_version.py', '--parts', check: true).stdout().strip().split('\n')
  pair = line.split('=')
  value = pair.length() > 1 ? pair[1] : ''
  version_parts.set('SCLI_VERSION_' + pair[0].to_upper(), value)
  version_values += {pair[0]: value}
endforeach

version_c = configure_file(
  input: 'version.c.in',
  output: 'version.c',
  configuration: version_parts,
)

# The release components drive the library version; the soname is the major,
# so every ABI break is a major release.
sparkcli_version_core = '@0@.@1@.@2@'.format(
  version_values['major'], version_values['minor'], version_values['patch'])

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
sparkcli_sources += version_c

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
# library() honours -Ddefault_library=static|shared|both. Only declarations
# marked SCLI_API (api.h) leave the shared object.
sparkcli_lib = library(
  'sparkcli',
  sources: sparkcli_sources,
  include_directories: inc_dirs,
  version: sparkcli_version_core,
  soversion: version_values['major'],
  gnu_symbol_visibility: 'hidden',
  c_args: ['-DSCLI_BUILDING_DLL'],
  install: install_sparkcli
)

# Install headers
if install_sparkcli
  install_headers(
    'api.h',
    'version.h',
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

meson.override_dependency('sparkcli', sparkcli_dep)

pkgconfig = import('pkgconfig')
pkgconfig.generate(
  sparkcli_lib,
  name: 'sparkcli',
  description: 'Get the most out of your terminal: colors, tables, trees, progress bars and more, in pure C.',
  url: 'https://github.com/antoniovazquezblanco/sparkcli',
  subdirs: 'sparkcli',
)
