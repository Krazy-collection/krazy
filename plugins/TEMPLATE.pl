#!/usr/bin/perl -w

eval 'exec /usr/bin/perl -w -S $0 ${1+"$@"}'
  if 0;    # not running under some shell
###############################################################################
# Sanity check plugin for the Krazy project.                                  #
# <SPDX-FileCopyrightText>                                                    #
# <SPDX-License-Identifier>                                                   #
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

use warnings;
use strict;
use FindBin qw($Bin);
use lib "$Bin/../../../../lib";
use Krazy::PreProcess;
use Krazy::Utils;

my ($Prog)    = "<plugin>";
my ($Version) = "<version>";

&parseArgs();

&Help()    if &helpArg();
&Version() if &versionArg();
&Explain() if &explainArg();
if ($#ARGV != 0) {&Help(); Exit 0;}

my ($f) = $ARGV[0];

# open file and slurp it in
open my $fh, '<:encoding(UTF-8)', $f or die;
my (@data_lines) = <$fh>;
close($fh);

# possibly post-process each line (remove-comments, etc)
# @lines = &postProcess(@data_lines)

# Check Condition
#my($linecnt) = 0;
#my($lstr) = "";
#foreach my ($line) (@lines) {
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

# Handle Check Results
#if (!$cnt) {
#  print "okay\n" if (!&quietArg());
#  Exit 0;
#} else {
#  print "$lstr ($cnt)\n" if (!&quietArg());
#  Exit $cnt;
#}

sub Help
{
  print "Check for <condition>\n";
  Exit 0 if &helpArg();
}

sub Version
{
  print "$Prog, version $Version\n";
  Exit 0 if &versionArg();
}

sub Explain
{
  print "<describe problem with solution.>\n";
  Exit 0 if &explainArg();
}
