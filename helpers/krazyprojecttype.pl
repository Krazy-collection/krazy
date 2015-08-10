#!/usr/bin/perl -w

###############################################################################
# Guesses the Type of a Project for Krazy                                     #
# Copyright 2015 by Allen Winter <winter@kde.org>                             #
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

# Purpose: mainly guess the project type in order to use the proper checkset.

# Takes one command line argument: the fullpath to the top of the project.
# Prints a supported Krazy checkset

use strict;
use File::Basename;
use File::Glob ':bsd_glob';
use FindBin qw($Bin);
use lib "$Bin/../../../lib";
use Krazy::Utils;

my($dir) = $ARGV[0];

if (! -d $dir) {
  print "sorry, no such dir $dir. Exiting...\n";
  exit 1;
}

printf("%s\n", &guessCheckSet($dir));
