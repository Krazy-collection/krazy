#!/usr/bin/perl -w

###############################################################################
# Guesses the Type of a Project for Krazy                                     #
# SPDX-FileCopyrightText: 2015 Allen Winter <winter@kde.org>                  #
# SPDX-License-Identifier: GPL-2.0-or-later                                   #
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

my ($dir) = $ARGV[0];

if (!-d $dir) {
  print "sorry, no such dir $dir. Exiting...\n";
  exit 1;
}

printf("%s\n", &guessCheckSet($dir));
