project('SparkCLI', 'c',
  license: 'GPL-3.0-or-later',
  version : run_command('git', '-C', meson.project_source_root(), 'describe', '--tags', '--always', check: false).stdout().strip(),
)

subdir('src')
subdir('examples')
subdir('assets')

tests_opt = get_option('tests')
if tests_opt.enabled() or (tests_opt.auto() and not meson.is_subproject())
  subdir('tests')
endif
