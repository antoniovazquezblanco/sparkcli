project('SparkCLI', 'c',
  license: 'GPL-3.0-or-later',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
  version: run_command('meson_version.py', check: true).stdout().strip(),
  meson_version: '>= 1.1',
)

# Record the version inside `meson dist` tarballs, which have no git history.
meson.add_dist_script('meson_version.py', '--dist', meson.project_version())

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
