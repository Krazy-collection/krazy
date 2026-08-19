# Krazy - Sanity check your source code

## Introduction

Krazy scans source code looking for issues that should be fixed
for reasons of policy, good coding practice, optimization, or any other
good reason.  In typical use, Krazy simply counts up the issues
and provides the line numbers where those issues occurred in each
file processed.  With the verbose option, the offending content will
be printed as well.

Krazy uses "sanity checker programs" which are small plugin programs
to do the real work of the scanning.  It is easy to write your own plugins
and tell Krazy how to use them.

Krazy has advantages over some of its competitors (see [Alternatives](#alternatives)), like:

- no compiling needed
- runs on all file types (more can always be added) in 1 tool
- not too difficult to write your own checkers using a scripting language of your choice,
e.g. `perl`, `python`. `ruby`, ...

and disadvantages:

- many of the checkers are quite dumb and rely on lots of regular expressions;
  false positives are more likely compared to tools that use a real parser.
- speed was never a consideration. yes, Krazy can be quite slow.

Many checkers included with Krazy are specific to KDE coding standards, while others can be
out-performed by [alternatives](#alternatives). Krazy provides "check-sets" to help the
user target the checkers best suited for their project.  For example, a vanilla C++ project
could use the check-sets "foss" and "c++", while a Qt-based C++ commercial project might
opt for the "qt" check-set.  KDE developers would use the "kde" check-set.

## Installation

See the [INSTALL instructions](INSTALL.md).

## Documentation

The documentation is included with the installed man pages.
See the krazy2(1), krazy2all(1) and krazyrc(3) man pages.

## Resources

Krazy is hosted by GitHub at <https://github.com/Krazy-collection/krazy>,
there you will find a wiki <https://github.com/Krazy-collection/krazy/wiki>
and an issue tracker <https://github.com/Krazy-collection/krazy/issues>.

Visit the Krazy home page at <https://krazy-collection.github.io/krazy>

Donate Plugins!  We'll include your cool plugin with this package if it
has an OSS-approved license!

## Alternatives

Consider using these tools in addition too or instead of Krazy.

[This list is not intended to be all-inclusive or to exclude other great tools; these are simply
my current favorites.]

- C/C++ source code

  - [cppcheck](https://cppcheck.sourceforge.io) - Tool for static C/C++ code analysis.
      Run `cppcheck --doc` to list all available checks.

  - [clang-tidy](https://clang.llvm.org/extra/clang-tidy) - A clang-based C++ "linter" tool.
      Run `clang-tidy --dump-config --checks=*` to list all available checks.

  - [clazy](https://invent.kde.org/sdk/clazy) - A C++ compiler plugin which allows clang to understand Qt semantics
      Run `clazy --list` to list all available checks.

- Freedesktop.org desktop files

  - [desktop-file-validate](https://www.freedesktop.org/wiki/Software/desktop-file-utils) - Tool to validate desktop
      entry files.

- QML

  - [qmllint](https://doc.qt.io/qt-6/qtqml-tooling-qmllint.html) - Tool to verify the syntactic validity of QML files.

- XML

  - [xmllint](https://gitlab.gnome.org/GNOME/libxml2/-/wikis/home) - Tool to validate XML files.

- Misc

  - [reuse](https://codeberg.org/fsfe/reuse-tool) - Tool for compliance with the REUSE recommendations.

  - [codespell](https://github.com/codespell-project/codespell) - Fix common misspellings in text files.

- Any file type

  - [pre-commit hooks](https://pre-commit.com) - Git hooks for managing and maintaining multi-language pre-commit hooks.
    See <https://pre-commit.com/hooks.html> to find available hooks.

## License

Copyright (C) 2005 by Allen Winter <winter@kde.org>

See the included [LICENSE.txt](LICENSE.txt) file.
