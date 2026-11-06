project('SparkCLI', 'c',
  license: 'GPL-3.0-or-later',
  version : run_command('git', '-C', meson.project_source_root(), 'describe', '--tags', '--always', check: false).stdout().strip(),
)

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
