project('desktop-file-utils',
  'c',
  version: '0.28',
  meson_version: '>=0.61.0',
  default_options: [
    'c_std=gnu11',
  ],
)

###############################################################################
# Project configuration

cc = meson.get_compiler('c')

config = configuration_data()

config.set_quoted('DATADIR', get_option('prefix') / get_option('datadir'))
config.set_quoted('VERSION', meson.project_version())


###############################################################################
# Compiler options

custom_cflags = [
  '-Wchar-subscripts',
  '-Wmissing-declarations',
  '-Wmissing-prototypes',
  '-Wnested-externs',
  '-Wpointer-arith',
  '-Wcast-align',
  '-Wsign-compare',

  '-Wno-unused-parameter',
]

if get_option('ansi')
  custom_cflags += [
    '-ansi',
    '-pedantic',
  ]
endif

if get_option('gcov')
  coverage_cflags = [
    '-fprofile-arcs',
    '-ftest-coverage',
  ]
  if cc.get_supported_arguments(coverage_cflags) != coverage_cflags
    error('Coverage is not supported by this compiler')
  endif
  custom_cflags += coverage_cflags
endif

add_project_arguments(cc.get_supported_arguments(custom_cflags),
  language: [ 'c', ],
)

check_functions = [
  { 'f': 'pledge', 'm': 'HAVE_PLEDGE' },
]
foreach check : check_functions
  config.set(check.get('m'), cc.has_function(check.get('f')))
endforeach


###############################################################################
# Dependencies

glib = dependency('glib-2.0', version: '>=2.26')
gio = dependency('gio-2.0', version: '>=2.26')

###############################################################################

subdir('man')
subdir('misc')
subdir('src')

install_symlink(
  'desktop-file-edit',
  install_dir: 'bin',
  pointing_to: 'desktop-file-install'
)

# This script writes git log to Changelog.
run_target('changelog', command: 'changelog.py')

meson.add_dist_script('changelog.py')
