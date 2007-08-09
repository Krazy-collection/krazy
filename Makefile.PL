# krazy Makefile.PL
#
# Copyright (C) 2005-2006 by Allen Winter <winter@kde.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See th
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


require 5.005;
use ExtUtils::MakeMaker;

WriteMakefile(
	      'NAME'		=> 'krazy2',
	      'AUTHOR'          => 'Allen Winter <winter@kde.org>',
	      'VERSION_FROM'    => 'krazy2',
	      'ABSTRACT_FROM'   => 'krazy2',
	      'EXE_FILES'       => ['krazy2','krazy2all','krazy2ebn'],
	      'PREFIX'          => '/usr/local',
	      'NORECURS'        => 1,
	      'PREREQ_FATAL'    => true,
	      'PREREQ_PM'       => { "HTML::Parser" => 2.20,
				     "File::Find"   => 1.01,
				     "Tie::IxHash"  => 1.20,
				     "XML::LibXML"  => 1.57
				   },
	      'dist'		=> { COMPRESS => 'gzip',
				     SUFFIX => '.gz', },
	     );
