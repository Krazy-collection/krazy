#!/usr/bin/perl -w

eval 'exec /usr/bin/perl -w -S $0 ${1+"$@"}'
    if 0; # not running under some shell
###############################################################################
# Sanity check plugin for the Krazy project.                                  #
# Copyright (C) <year> by <your_name> <your_email_address>                    #
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

# Tests <filetype> source for <condition>

# Program options:
#   --help:          print one-line help message and exit
#   --version:       print one-line version information and exit
#   --priority:      report issues of the specified priority only
#   --strict:        report issues with the specified strictness level only
#   --check-sets:    list of checksets
#   --explain:       print an explanation with solving instructions
#   --installed      file is to be installed
#   --quiet:         suppress all output messages
#   --verbose:       print the offending content

# Exits with status=0 if test condition is not present in the source;
# else exits with the number of failures encountered.

use strict;
use FindBin qw($Bin);
use lib "$Bin/../../../../lib";
use Krazy::PreProcess;
use Krazy::Utils;

my($Prog) = "<plugin>";
my($Version) = "<version>";

&parseArgs();

&Help() if &helpArg();
&Version() if &versionArg();
&Explain() if &explainArg();
if ($#ARGV != 0){ &Help(); Exit 0; }

# Check Condition
#my($f) = $ARGV[0];
#open(F, "$f") || die "Couldn't open $f";
#my($cnt) = 0;
#my($linecnt) = 0;
#my($line);
#my($lstr) = "";
#while ($line = <F>) {
#  $linecnt++;
#  if ($line =~ m/SOMETHING/) {
#    $cnt++;
#    if ($cnt == 1) {
#      $lstr = "line\#" . $linecnt;
#    } else {
#      $lstr = $lstr . "," . $linecnt;
#    }
#    print "=> $line\n" if (&verboseArg());
#  }
#}
#close(F);

# Handle Check Results
#if (!$cnt) {
#  print "okay\n" if (!&quietArg());
#  Exit 0;
#} else {
#  print "$lstr ($cnt)\n" if (!&quietArg());
#  Exit $cnt;
#}

sub Help {
  print "Check for <condition>\n";
  Exit 0 if &helpArg();
}

sub Version {
  print "$Prog, version $Version\n";
  Exit 0 if &versionArg();
}

sub Explain {
  print "<describe problem with solution.>\n";
  Exit 0 if &explainArg();
}
