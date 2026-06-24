project('SparkCLI', 'c',
  license: 'GPL-3.0-or-later',
  version : run_command('git', '-C', meson.project_source_root(), 'describe', '--tags', '--always', check: false).stdout().strip(),
)

subdir('src')
subdir('examples')

pkg = import('pkgconfig')

pkg.generate(
  sparkcli_lib,
  name : 'sparkcli',
  description : 'SparkCLI terminal formatting library',
  filebase : 'sparkcli',
  subdirs : 'sparkcli',
  version : meson.project_version(),
)
