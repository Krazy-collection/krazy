#!/usr/bin/env python3

import os

os.symlink(
    'desktop-file-install',
    os.getenv('MESON_INSTALL_DESTDIR_PREFIX') + '/bin/desktop-file-edit'
)
