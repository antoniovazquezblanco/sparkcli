test_c_args = []
if host_machine.system() != 'windows'
  test_c_args += '-D_POSIX_C_SOURCE=200809L'
endif

# The version script is Python, so this one runs outside the cmocka suite and
# stays available even when cmocka is not.
test(
  'version_script',
  import('python').find_installation(),
  args: [files('version_script_test.py')],
  suite: 'unit',
)

cmocka_dep = dependency('cmocka', required: get_option('tests').enabled())

if not cmocka_dep.found()
  message('cmocka not found: skipping unit tests.')
  subdir_done()
endif

# Tests that reach library internals link those objects directly: the symbols
# are hidden in the shared library, so going through it would not resolve.
internal_test_objects = {
  'color': ['utils/env.c'],
}

unit_tests = [
  'color',
  'linkage',
  'math',
  'str',
  'strdup',
  'table',
  'table_align',
  'tree',
  'version',
]

# C++ is optional: a C library must not require a C++ compiler to build.
if add_languages('cpp', required: false, native: false)
  test(
    'cxx_headers',
    executable(
      'test_cxx_headers',
      'cxx_headers_test.cpp',
      dependencies: [sparkcli_dep],
      install: false,
    ),
    suite: 'unit',
  )
endif

foreach unit_test : unit_tests
  internal_sources = internal_test_objects.get(unit_test, [])
  test_objects = internal_sources.length() > 0 ? sparkcli_lib.extract_objects(internal_sources) : []

  test(
    unit_test,
    executable(
      'test_' + unit_test,
      'test_' + unit_test + '.c',
      objects: test_objects,
      dependencies: [sparkcli_dep, cmocka_dep],
      c_args: test_c_args,
      install: false,
    ),
    suite: 'unit',
  )
endforeach
