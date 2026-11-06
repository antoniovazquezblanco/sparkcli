cmocka_dep = dependency('cmocka', required: get_option('tests').enabled())

if not cmocka_dep.found()
  message('cmocka not found: skipping unit tests.')
  subdir_done()
endif

unit_tests = [
  'color',
  'linkage',
  'str',
  'table',
  'table_align',
  'tree',
]

foreach unit_test : unit_tests
  test(
    unit_test,
    executable(
      'test_' + unit_test,
      'test_' + unit_test + '.c',
      dependencies: [sparkcli_dep, cmocka_dep],
      install: false,
    ),
    suite: 'unit',
  )
endforeach
