# krazy Makefile.PL
#
# SPDX-FileCopyrightText: 2005 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

require 5.005;
use ExtUtils::MakeMaker;

WriteMakefile(
              'NAME'            => 'krazy2',
              'AUTHOR'          => 'Allen Winter <winter@kde.org>',
              'VERSION_FROM'    => 'krazy2',
              'ABSTRACT_FROM'   => 'krazy2',
              'EXE_FILES'       => ['krazy2','krazy2all'],
              'MAN3PODS'        => { 'docs/krazyrc.pod' => 'blib/man3/krazyrc.3' },
              'NORECURS'        => 1,
              'PREREQ_FATAL'    => true,
              'PREREQ_PM'       => { "HTML::Parser" => 2.20,
                                     "File::Find"   => 1.01,
                                     "Tie::IxHash"  => 1.20,
                                     "XML::LibXML"  => 1.57,
                                     "YAML"         => 0.68,
                                     "JSON"         => 2.90,
                                     "JSON::XS"     => 2.90,
                                     "Env"          => 1.00,
                                   },
              'dist'            => { COMPRESS => 'gzip',
                                     SUFFIX => '.gz', },
             );
