#!/usr/bin/perl -w

###############################################################################
# Prints a the list of files krazy2all, etc will process.                     #
# SPDX-FileCopyrightText: 2017 Allen Winter <winter@kde.org>                  #
# SPDX-License-Identifier: GPL-2.0-or-later                                   #
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
