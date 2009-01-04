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

#create a symlink to the perl modules so we can find them
V=`perl -e 'printf "%vd", $^V'`
if ( test -d $TOP/share/perl5/site_perl ) then
  (cd $TOP/lib; rm -f Krazy; ln -s $TOP/share/perl5/site_perl/$V/Krazy Krazy)
else
  if ( test -d $TOP/share/perl ) then
    (cd $TOP/lib; rm -f Krazy; ln -s $TOP/share/perl/$V/Krazy Krazy)
  else
    if ( test -d $TOP/lib/perl5/site_perl ) then
      (cd $TOP/lib; rm -f Krazy; ln -s $TOP/lib/perl5/site_perl/$V/Krazy Krazy)
    else
      if ( test -d $TOP/lib64/perl5/site_perl ) then
        (cd $TOP/lib; rm -f Krazy; ln -s $TOP/lib64/perl5/site_perl/$V/Krazy Krazy)
      else
        echo
	echo "==================================================================="
	echo "Unknown perl installation issue encountered. Aborting installation."
	echo "Please contact winter@kde.org about this."
	echo "==================================================================="
	exit 1
      fi
    fi
  fi
fi

#install helper scripts
cd helpers && \
make install PREFIX=$TOP && \
make realclean
cd ..

#install binary helper scripts
if ( test $EBN -eq 0 ) then
  cd src/desktop-file-utils-0.14 && \
  ./configure && make && \
  cp -f ./src/desktop-file-validate $TOP/lib/krazy2/krazy-helpers && \
  make distclean
  cd ../..
else
  if ( test ! -x /usr/local/bin/desktop-file-validate ) then
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
make install INSTALL_ROOT=$TOP && \
make distclean
cd ..

# build and install cppcheck plugins
cd cppchecks && \
mkdir -p build && \
cd build && \
cmake -DCMAKE_PREFIX_PATH=$TOP .. && \
make && \
make install
cd ..
rm -rf build
cd ..

#install share stuff
mkdir -p $TOP/share/dtd
cd share && \
cp kpartgui.dtd $TOP/share/dtd
cp kcfg.dtd $TOP/share/dtd
cd ..

#build and install extra docs
cd doc && \
make install PREFIX=$TOP && \
make realclean
cd ..

#remove old checkers no longer in use
rm -f $TOP/lib/krazy2/krazy-plugins/*/contractions

#permissions
if ( test $EBN -eq 1 ) then
  chmod ug+w $TOP/bin/*
  chmod ug+w $TOP/lib/krazy2/krazy-plugins/*
  chmod ug+w $TOP/lib/krazy2/krazy-plugins/*/*
  chmod ug+w $TOP/lib/krazy2/krazy-extras/*
  chmod ug+w $TOP/lib/krazy2/krazy-extras/*/*
  chmod ug+w $TOP/lib/krazy2/krazy-helpers/*
fi
