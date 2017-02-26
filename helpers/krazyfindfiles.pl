#!/usr/bin/perl -w

###############################################################################
# Prints a the list of files krazy2all, etc will process.                     #
# Copyright 2017 by Allen Winter <winter@kde.org>                             #
#                                                                             #
# This program is free software; you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation; either version 2 of the License, or           #
# (at your option) any later version.                                         #
#                                                                             #
# This program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program; if not, write to the Free Software Foundation, Inc.,     #
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.               #
#                                                                             #
###############################################################################

# Purpose: mainly see if the findFiles function works as intended.

# Takes one command line argument: the fullpath to the top of the project.
# Prints the list of files

use strict;
use FindBin qw($Bin);
use lib "$Bin/../../../lib";
use Krazy::Utils;

my($dir) = $ARGV[0];

if (! -d $dir) {
  print "sorry, no such dir $dir. Exiting...\n";
  exit 1;
}

my($file);
foreach $file (split('\s', &findFiles($dir))) {
  $file =~ s+^$dir+\.+gm;
  $file =~ s+\\n+\'\\n\'+g;
  printf "$file\n";
}
