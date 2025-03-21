desktop_file_edit_man = configure_file(
  input : 'desktop-file-install.1',
  output: 'desktop-file-edit.1',
  command: ['cp', '@INPUT@', '@OUTPUT@']
)

install_man([
  'desktop-file-validate.1',
  'desktop-file-install.1',
  'update-desktop-database.1',
  desktop_file_edit_man,
])
