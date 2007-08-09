#!/bin/sh

# install krazy

#change TOP to whatever you like for your top-level installation directory
if ( test `hostname | egrep -c englishbreakfast` -gt 0 ) then
  EBN=1
  TOP=/usr/local/www/data-ebn/krazy
  QMAKE=/usr/local/src/qt-copy/bin/qmake
else
  EBN=0
  TOP=/usr/local/Krazy2
  # Assumed to be in the PATH
  QMAKE=qmake
fi

#or pass TOP in as the first command line option
if ( test $# -gt 0 ) then
  TOP=$1
fi

if ( test `$QMAKE -query QT_VERSION | egrep -c '^4'` -eq 0 ) then
  echo "The qmake found in your \$PATH is not the Qt4 version"
  echo "Please put your Qt4 bin directory first. Exiting..."
  exit 1
fi

if ( test ! -d "$TOP" ) then
  echo "Installation dir \"$TOP\" does not exist. Creating..."
  mkdir -p $TOP || exit 1
fi

#install user-interface scripts
perl Makefile.PL PREFIX=$TOP
stat=$?
if ( test $stat -ne 0 ) then
  exit $stat
fi
make && \
make install && \
make realclean

#install helper scripts
cd helpers && \
make install PREFIX=$TOP && \
make realclean
cd ..

#build and install non-binary plugins
cd plugins && \
make install PREFIX=$TOP && \
make realclean
cd ..

#build and install binary plugins
cd src && \
$QMAKE && \
make && \
make install INSTALL_ROOT=$TOP && \
make distclean
cd ..

#permissions
if ( test $EBN -eq 1 ) then
  chmod ug+w $TOP/bin/*
  chmod ug+w $TOP/libexec/krazy-plugins/*
fi
