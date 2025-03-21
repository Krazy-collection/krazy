configure_file(
  output: 'config.h',
  configuration: config,
)

desktop_file_lib = static_library('desktop_file',
  'keyfileutils.c',
  'mimeutils.c',
  'validate.c',
  dependencies: [glib, gio],
)

executable('desktop-file-validate',
  'validator.c',
  link_with: desktop_file_lib,
  dependencies: glib,
  install: true,
)

executable('desktop-file-install',
  'install.c',
  link_with: desktop_file_lib,
  dependencies: glib,
  install: true,
)

executable('update-desktop-database',
  'update-desktop-database.c',
  link_with: desktop_file_lib,
  dependencies: glib,
  install: true,
)
