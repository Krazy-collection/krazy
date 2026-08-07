#!/bin/bash

# SPDX-FileCopyrightText: Allen Winter <winter@kde.org>
# SPDX-License-Identifier: MIT

# install krazy

#Exit if any undefined variable is used.
set -u
#Exit this script if it any subprocess exits non-zero.
set -e
#If any process in a pipeline fails, the return value is a failure.
set -o pipefail

COMMAND_EXISTS() {
  set +e
  command -v "$1" >/dev/null 2>&1
  if (test $? != 0); then
    echo "$1 is not in your PATH"
    if (test $# -gt 0); then
      shift
      echo "$@"
    else
      echo "Must install this program before continuing"
    fi
    exit 1
  fi
  set -e
}

#make sure some helper programs exist before continuing
COMMAND_EXISTS desktop-file-validate "Please install the desktop-file-utils package or install it from source"

#bootstrap by checking that the MakeMaker module is installed
module="ExtUtils::MakeMaker"
perl -M$module -e 1 2>/dev/null
status=$?
if (test $status -ne 0); then
  echo "Cannot locate the $module Perl module"
  echo "Please install this module... exiting"
  exit 1
fi

#save current working dir
savedir=$(pwd)

#change TOP to whatever you like for your top-level installation directory
TOP=/usr/local/Krazy2

#or pass TOP in as the first command line option
if (test $# -gt 0); then
  TOP=$1
fi

if test "$TOP" = "--help"; then
  echo "Usage: $0 [installation_prefix]"
  exit 1
fi

if (test ! -d "$TOP"); then
  echo "Installation dir \"$TOP\" does not exist. Creating..."
  mkdir -p "$TOP" || exit 1
fi
if (test ! -w "$TOP"); then
  echo "Installation dir \"$TOP\" is not writable by the current user. Exiting..."
  exit 1
fi

#install user-interface scripts
#perl Makefile.PL INSTALL_BASE=$TOP
perl Makefile.PL INSTALL_BASE="$TOP" INSTALLSITESCRIPT="$TOP/bin" INSTALLSITEBIN="$TOP/bin" INSTALLSITELIB="$TOP/lib" INSTALLSITEMAN1DIR="$TOP/share/man/man1" INSTALLSITEMAN3DIR="$TOP/share/man/man3"
stat=$?
if (test $stat -ne 0); then
  exit $stat
fi
make &&
  make install &&
  make realclean

if (test ! -d "$TOP/lib"); then
  echo "==================================================================="
  echo "Unknown Perl installation issue encountered. Aborting installation."
  echo "Please contact winter@kde.org about this."
  echo "==================================================================="
  exit 1
fi

#following is needed on ArchLinux
if (test -d "$TOP/bin/perlbin/site"); then
  (
    cd "$TOP/bin/perlbin/site" || exit 0
    mv ./* ../..
  )
  rm -rf "$TOP/bin/perlbin"
fi

if (test -d "$TOP/local/bin"); then
  mkdir -p "$TOP/bin"
  (
    cd "$TOP/local/bin"
    mv ./* ../../bin
  )
  rm -rf "$TOP/local/bin"
fi

#install helper scripts
cd helpers &&
  make install PREFIX="$TOP" &&
  make realclean
cd ..

#build and install plugins
cd plugins &&
  make install PREFIX="$TOP" &&
  make realclean
cd ..

#build and install extras
cd extras &&
  make install PREFIX="$TOP" &&
  make realclean
cd ..

#build and install sets
cd sets &&
  make install PREFIX="$TOP" &&
  make realclean
cd ..

#install share stuff
mkdir -p "$TOP/share/dtd"
cd share
cp kpartgui.dtd "$TOP/share/dtd"
cp kxmlgui.xsd "$TOP/share/dtd"
cp kcfg.xsd "$TOP/share/dtd"
cd ..

#install config file
mkdir -p "$TOP/conf"
cp conf/settings.yaml "$TOP/conf"

#create a krazy2rc man page (a symlink to the krazyrc man page)
if (test -d "$TOP/share/man/man3"); then
  (
    cd "$TOP/share/man/man3"
    ln -sf krazyrc.3 krazy2rc.3
  )
fi

#remove old checkers no longer in use
rm -f "$TOP"/lib/krazy2/krazy-plugins/*/contractions #moved to extras
rm -f "$TOP"/lib/krazy2/krazy-plugins/*/qconnect
rm -f "$TOP"/lib/krazy2/krazy-plugins/*/foreach "$TOP"/lib/krazy2/krazy-sets/*/*-foreach
rm -f "$TOP"/lib/krazy2/krazy-plugins/*/passbyvalue
rm -f "$TOP"/lib/krazy2/krazy-plugins/*/qmlsyntax

#final cleaning
cd "$savedir"
if (test -f Makefile); then
  make -s clean
  rm -f Makefile.old
fi
