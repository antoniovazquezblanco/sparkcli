# Documentation asset generation.
# Renders the VHS tapes in tapes/ into images under img/.

vhs = find_program('vhs', required: false)
if not vhs.found()
  warning('vhs not found: skipping documentation asset generation (install https://github.com/charmbracelet/vhs to enable it).')
  subdir_done()
endif

fs = import('fs')

asset_targets = []
foreach example : examples
  if not fs.is_file('tapes' / example + '.tape')
    continue
  endif

  output = example + '.gif'
  demo_dependency = get_variable(example + '_demo')

  asset_targets += custom_target(
    'asset_' + example,
    input: files('tapes' / example + '.tape'),
    output: output,
    command: ['vhs', '@INPUT@', '--output', '@OUTPUT@'],
    depends: demo_dependency,
    build_by_default: false,
    console: true,
  )
endforeach

# Convenience aggregate: `meson compile -C <builddir> assets`.
alias_target('assets', asset_targets)
