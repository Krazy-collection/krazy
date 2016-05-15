#!/bin/sh

# install krazy

#Exit this script if it any subprocess exits non-zero.
set -e

#save current working dir
savedir=`pwd`

#bootstrap by checking that the MakeMaker module is installed
module="ExtUtils::MakeMaker"
perl -M$module -e 1 2> /dev/null
status=$?
if ( test $status -ne 0 ) then
  echo "Cannot locate the $module Perl module"
  echo "Please install this module... exiting"
  exit 1
fi

#change TOP to whatever you like for your top-level installation directory
if ( test `hostname | egrep -c www` -gt 0 ) then
  EBN=1
  TOP=/mnt/ebn
  QMAKE=/usr/bin/qmake
else
  EBN=0
  TOP=/usr/local/Krazy2
  if ( test -z "$QMAKE") then
    # Assumed to be in the PATH
    QMAKE=qmake
  fi
fi

#or pass TOP in as the first command line option
if ( test $# -gt 0 ) then
  TOP=$1
fi

if test "$TOP" = "--help"; then
  echo "Usage: $0 [installation_prefix]";
  exit 1
fi

qmakever=`$QMAKE -query QT_VERSION`
if ( test `echo $qmakever | grep -ic unknown` -eq 0 )
then
  qmake_majver=`echo $qmakever | cut -d. -f1`
  qmake_minver=`echo $qmakever | cut -d. -f2`
else
  qmake_majver=3
  qmake_minver=0
fi
if ( test $qmake_majver -lt 4) then
  echo "The qmake found in your \$PATH is too old (must be Qt4 or higher)."
  echo "You can solve this by:"
  echo "- prepending a modern Qt bin directory to your \$PATH"
  echo "- setting \$QMAKE to the name of a modern Qt version of qmake"
  echo "Exiting..."
  exit 1
fi

if ( test $qmake_majver -lt 5 -o $qmake_majver -eq 5 -a $qmake_minver -lt 3 )
then
  echo "WARNING: You are using Qt version less than 5.3.0."
  echo "WARNING: You need at least Qt version 5.3.0 to build the qmlsyntax checker."
  echo
  echo "You can solve this by:"
  echo "- prepending your Qt5.3 bin directory to your \$PATH"
  echo "- setting \$QMAKE to the name of the Qt5.3 version of qmake"
  echo
  echo "Continuing with the installation..."
fi

if ( test ! -d "$TOP" ) then
  echo "Installation dir \"$TOP\" does not exist. Creating..."
  mkdir -p $TOP || exit 1
fi

#install user-interface scripts
#perl Makefile.PL PREFIX=$TOP
perl Makefile.PL INSTALLSITESCRIPT=$TOP/bin INSTALLSITEBIN=$TOP/bin INSTALLSITELIB=$TOP/lib PREFIX=$TOP
stat=$?
if ( test $stat -ne 0 ) then
  exit $stat
fi
make && \
make install && \
make realclean

if ( test ! -d "$TOP/lib" ) then
  echo "==================================================================="
  echo "Unknown Perl installation issue encountered. Aborting installation."
  echo "Please contact winter@kde.org about this."
  echo "==================================================================="
  exit 1
fi

#following is needed on ArchLinux
if ( test -d $TOP/bin/perlbin/site ) then
  (cd $TOP/bin/perlbin/site; mv * ../..)
  rm -rf $TOP/bin/perlbin
fi

#following is needed on Mandriva
if ( test -d $TOP/local/bin ) then
  mkdir -p $TOP/bin
  (cd $TOP/local/bin; mv * ../../bin)
  rm -rf $TOP/local/bin
fi

#install helper scripts
cd helpers && \
make install PREFIX=$TOP && \
make realclean
cd ..

#install binary helper scripts
if ( test $EBN -eq 0 ) then
  cd src/desktop-file-utils-0.22 && \
  touch configure.ac aclocal.m4 configure Makefile.am Makefile.in && \
  sh configure && make && \
  cp -f ./src/desktop-file-validate $TOP/lib/krazy2/krazy-helpers && \
  make distclean; rm -rf autom4te.cache
  cd ../..
else
  if ( test ! -x /usr/bin/desktop-file-validate ) then
    echo "Please install the desktop-file-utils package"
    echo "Exiting $0"
    exit 1
  fi
fi

#build and install non-binary plugins
cd plugins && \
make install PREFIX=$TOP && \
make realclean
cd ..

#build and install non-binary extras
cd extras && \
make install PREFIX=$TOP && \
make realclean
cd ..

#build and install (non-cppchecks) binary plugins
cd src && \
$QMAKE && \
make && \
make install INSTALL_ROOT=$TOP
make distclean
cd ..

# build and install cppcheck plugins
# TODO: Probably the whole cpp checks will move out of krazy because I think
#       that it makes more sense as a seperate tool. For it is certainly not
#       ready for use anyway so we comment it out.
# cd cppchecks && \
# mkdir -p build && \
# cd build && \
# cmake -DCMAKE_PREFIX_PATH=$TOP .. && \
# make && \
# make install
# cd ..
# rm -rf build
# cd ..

#build and install sets
cd sets && \
make install PREFIX=$TOP && \
make realclean
cd ..

#install share stuff
mkdir -p $TOP/share/dtd
cd share && \
cp kpartgui.dtd $TOP/share/dtd
cp kxmlgui.xsd $TOP/share/dtd
cp kcfg.dtd $TOP/share/dtd
cp kcfg.xsd $TOP/share/dtd
cd ..

mkdir -p $TOP/share/xsl
cd stylesheets && \
cp *.xsl $TOP/share/xsl
cd ..

#install config file
mkdir -p $TOP/conf
cp conf/settings.yaml $TOP/conf

#create a krazy2rc man page (a symlink to the krazyrc man page)
if ( test -d $TOP/share/man/man3 )
then
  (cd $TOP/share/man/man3; ln -sf krazyrc.3 krazy2rc.3)
fi

#remove old checkers no longer in use
rm -f $TOP/lib/krazy2/krazy-extras/*/contractions
rm -f $TOP/lib/krazy2/krazy-plugins/*/qconnect

#permissions
if ( test $EBN -eq 1 ) then
  chgrp kde $TOP/bin/*
  chmod ug+w $TOP/bin/*
  chgrp kde $TOP/lib/krazy2/krazy-plugins/*
  chmod ug+w $TOP/lib/krazy2/krazy-plugins/*
  chgrp kde $TOP/lib/krazy2/krazy-plugins/*/*
  chmod ug+w $TOP/lib/krazy2/krazy-plugins/*/*
  chgrp kde $TOP/lib/krazy2/krazy-extras/*
  chmod ug+w $TOP/lib/krazy2/krazy-extras/*
  chgrp kde $TOP/lib/krazy2/krazy-extras/*/*
  chmod ug+w $TOP/lib/krazy2/krazy-extras/*/*
  chgrp kde $TOP/lib/krazy2/krazy-helpers/*
  chmod ug+w $TOP/lib/krazy2/krazy-helpers/*
  chgrp kde $TOP/lib/krazy2/krazy-sets/*
  chmod ug+w $TOP/lib/krazy2/krazy-sets/*
  chgrp kde $TOP/lib/krazy2/krazy-sets/*/*
  chmod ug+w $TOP/lib/krazy2/krazy-sets/*/*
fi

#final cleaning
cd $savedir
if ( test -f Makefile )
then
  make -s clean
  rm -f Makefile.old
fi
