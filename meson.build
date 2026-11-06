# The git description is preferred so development builds carry the commit they
# came from. It comes back empty for a release tarball, a clone with no tags or
# a machine without git, so fall back to a literal rather than configuring a
# project with no version at all. Meson requires project() to be the first
# statement, so the command cannot be hoisted into a variable first.
project('SparkCLI', 'c',
  license: 'GPL-3.0-or-later',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
  version : run_command('git', '-C', meson.project_source_root(), 'describe', '--tags', '--always', check: false).stdout().strip() != ''
    ? run_command('git', '-C', meson.project_source_root(), 'describe', '--tags', '--always', check: false).stdout().strip()
    : '0.0.7',
)

# -Wwrite-strings is deliberately absent: it reports every string literal
# passed to the char * table and progress parameters, so it waits for those
# signatures to take const.
cc = meson.get_compiler('c')
add_project_arguments(cc.get_supported_arguments([
    '-Wvla',
    '-Wshadow',
    '-Wundef',
    '-Wpointer-arith',
    '-Wformat=2',
    '-Wcast-qual',
    '-Wmissing-prototypes',
    '-Wstrict-prototypes',
  ]), language: 'c')

subdir('src')

examples_opt = get_option('examples')
if examples_opt.enabled() or (examples_opt.auto() and not meson.is_subproject())
  subdir('examples')
  subdir('assets')
endif

tests_opt = get_option('tests')
if tests_opt.enabled() or (tests_opt.auto() and not meson.is_subproject())
  subdir('tests')
endif
