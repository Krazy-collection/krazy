#!/usr/bin/env python3

import os, subprocess
from pathlib import Path

changelog_path = Path(
    os.getenv('MESON_DIST_ROOT')
    or os.getenv('MESON_SOURCE_ROOT')
    or Path(__file__).parent
) / 'ChangeLog'

print('Writing changelog to {path}'.format(path=changelog_path))

with open(changelog_path, 'w') as changelog_file:
    changelog = subprocess.check_output([
        'git', 'log',
        '--no-color',
        '--find-renames',
        '--find-copies',
        '--name-status'
    ]).decode()
    changelog_file.write(changelog)
