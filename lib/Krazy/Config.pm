###############################################################################
# Sanity checks for your source code                                          #
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>                  #
# SPDX-License-Identifier: GPL-2.0-or-later                                   #
###############################################################################

#TODO:
# handle second args in EXCLUDE, CHECK, EXTRA directives

package Krazy::Config;

use warnings;
use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);
use Cwd;
use Krazy::Utils;

use Exporter;
$VERSION = 1.60;
@ISA     = qw(Exporter);

@EXPORT    = qw(ParseKrazyRC);
@EXPORT_OK = qw();

#==============================================================================
# .krazy file parsing
#
# Each line in the file can be a control directive, a blank line,
# or a comment. Comment lines start with the # character.
#
# Supports the following directives:
# EXCLUDE plugin1[,plugin2,...] <regexp>
# CHECK plugin1[,plugin2,...] <regexp>
# EXTRA plugin1[,plugin2,...] <regexp>
# CHECKSETS set1[,set2,...]
# TYPES type1[,type2,...]
# EXCLUDETYPES type1[,type2,...]
# SKIP regexp
# PRIORITY <low|normal|high|important|all>
# STRICT <normal|super|all>
# OUTPUT <quiet|brief|normal>
# EXPORT <text|textlist|textedit|gitlab>
# IGNORESUBS subdir1[,subdir2,...]
# EXTRASUBS subdir1[,subdir2,...]
# IGNOREMODS module1[,module2,...]
# CPP_INCLUDE_ORDER <true|false>
#
# Multiple directives may be specified per file; they will be combined in
# a logical way.
#
# The directive is case-insensitive.
#==============================================================================

my (
  $rcExclude,   $rcOnly,     $rcCheckSets, $rcExtra,  $rcIncTypes, $rcExcTypes,
  $rcSkipRegex, $rcPriority, $rcStrict,    $rcOutput, $rcExport
);
my (@rcIgSubsList, @rcExSubsList, @rcIgModsList);
my ($CWD);

sub ParseKrazyRC
{
  my ($rcfile) = @_;

  $CWD = getcwd;

  $rcExclude                    = "";
  $rcOnly                       = "";
  $rcCheckSets                  = "";
  $rcExtra                      = "";
  $rcIncTypes                   = "";
  $rcExcTypes                   = "";
  $rcSkipRegex                  = "";
  $rcPriority                   = "";
  $rcStrict                     = "";
  $rcOutput                     = "";
  $rcExport                     = "";
  @rcIgSubsList                 = ();
  @rcExSubsList                 = ();
  @rcIgModsList                 = ();
  $ENV{KRAZY_CPP_INCLUDE_ORDER} = "false";

  my (%directives);
  my ($linecnt, $directive, $arg);

  # open file and slurp it in
  open my $fh, '<:encoding(UTF-8)', $rcfile or return %directives;
  my (@lines) = <$fh>;
  close($fh);

  foreach my ($line) (@lines) {
    $linecnt++;
    $line =~ s/#.*//;          #strip comment
    $line =~ s/^\s+//;         #strip leading whitespace
    $line =~ s/\s+$//;         #strip trailing whitespace
    $line =~ s/\s*,\s*/,/g;    #remove whitespace around commas
    next if (!$line);

    ($directive, $arg) = split(" ", $line);
    my $saveDirective = $directive;
    $directive = uc($directive);
    if ($directive eq "EXTRA") {
      &extras($arg, $linecnt, $rcfile);
    } elsif ($directive eq "CHECK") {
      &checks($arg, $linecnt, $rcfile);
    } elsif ($directive eq "CHECKSETS") {
      &checkSets($arg, $linecnt, $rcfile);
    } elsif ($directive eq "EXCLUDE") {
      &excludes($arg, $linecnt, $rcfile);
    } elsif ($directive eq "IGNORESUBS") {
      &ignoreSubs($arg, $linecnt, $rcfile);
    } elsif ($directive eq "EXTRASUBS") {
      &extraSubs($arg, $linecnt, $rcfile);
    } elsif ($directive eq "IGNOREMODS") {
      &ignoreMods($arg, $linecnt, $rcfile);
    } elsif ($directive eq "SKIP") {
      &skips($arg, $linecnt, $rcfile);
    } elsif ($directive eq "PRIORITY") {
      &priority($arg, $linecnt, $rcfile);
    } elsif ($directive eq "STRICT") {
      &strict($arg, $linecnt, $rcfile);
    } elsif ($directive eq "OUTPUT") {
      &output($arg, $linecnt, $rcfile);
    } elsif ($directive eq "EXPORT") {
      &export($arg, $linecnt, $rcfile);
    } elsif ($directive eq "TYPES") {
      &types($arg, $linecnt, $rcfile);
    } elsif ($directive eq "EXCLUDETYPES") {
      &excludeTypes($arg, $linecnt, $rcfile);
    } elsif ($directive eq "CPP_INCLUDE_ORDER") {
      &cppIncludeSettings($directive, $arg, $linecnt, $rcfile);
    } else {
      print "$rcfile: Invalid directive \"$saveDirective\" (line $linecnt)\n";
      close(F);
      exit 1;
    }
  }
  close(F);

  #return a hash of the directives
  $directives{'EXCLUDE'}      = $rcExclude;
  $directives{'CHECK'}        = $rcOnly;
  $directives{'CHECKSETS'}    = $rcCheckSets;
  $directives{'EXTRA'}        = $rcExtra;
  $directives{'TYPES'}        = $rcIncTypes;
  $directives{'EXCLUDETYPES'} = $rcExcTypes;
  $directives{'SKIPREGEX'}    = $rcSkipRegex;
  $directives{'PRIORITY'}     = $rcPriority;
  $directives{'STRICT'}       = $rcStrict;
  $directives{'OUTPUT'}       = $rcOutput;
  $directives{'EXPORT'}       = $rcExport;
  @{$directives{'IGSUBSLIST'}}   = deDupe(@rcIgSubsList);
  @{$directives{'IGEXTRASLIST'}} = deDupe(@rcExSubsList);
  @{$directives{'IGMODSLIST'}}   = deDupe(@rcIgModsList);
  return %directives;
}

sub extras
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing EXTRA arguments, line $l, $f\n";
    exit 1;
  }
  if (!$rcExtra) {
    $rcExtra = $args;
  } else {
    $rcExtra .= "," . $args;
  }
}

sub checks
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing CHECK arguments, line $l, $f\n";
    exit 1;
  }
  if (!$rcOnly) {
    $rcOnly = $args;
  } else {
    $rcOnly .= "," . $args;
  }
}

sub checkSets
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing CHECKSETS arguments, line $l, $f\n";
    exit 1;
  }
  if (!$rcCheckSets) {
    $rcCheckSets = $args;
  } else {
    $rcCheckSets .= "," . $args;
  }
}

sub excludes
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing EXCLUDE arguments, line $l, $f\n";
    exit 1;
  }
  if (!$rcExclude) {
    $rcExclude = $args;
  } else {
    $rcExclude .= "," . $args;
  }
}

sub types
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing TYPES arguments, line $l, $f\n";
    exit 1;
  }
  if (!$rcIncTypes) {
    $rcIncTypes = $args;
  } else {
    $rcIncTypes .= "," . $args;
  }
}

sub excludeTypes
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing EXCLUDETYPES arguments, line $l, $f\n";
    exit 1;
  }
  if (!$rcExcTypes) {
    $rcExcTypes = $args;
  } else {
    $rcExcTypes .= "," . $args;
  }
}

sub cppIncludeSettings
{
  my ($s, $args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing $s arguments, line $l, $f\n";
    exit 1;
  }

  if ($s eq "CPP_INCLUDE_ORDER") {
    if (!&validateCppIncludeOrderType($args)) {
      my ($lst) = &cppIncludeOrderTypeStr();
      print "invalid CPP_INCLUDE_ORDER argument \"$args\", line $l, $f\nChoices are: $lst\n";
      exit 1;
    } else {
      $args = lc($args);
      if ($args eq "yes" || $args eq "on" || $args eq "true") {
        $ENV{KRAZY_CPP_INCLUDE_ORDER} = "true";
      } else {
        $ENV{KRAZY_CPP_INCLUDE_ORDER} = "false";
      }
    }
  }
}

sub ignoreSubs
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing IGNORESUBS arguments, line $l, $f\n";
    exit 1;
  }
  push(@rcIgSubsList, split(",", $args));
}

sub extraSubs
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing EXTRASUBS arguments, line $l, $f\n";
    exit 1;
  }
  push(@rcExSubsList, split(",", $args));
}

sub ignoreMods
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing IGNOREMODS arguments, line $l, $f\n";
    exit 1;
  }
  push(@rcIgModsList, split(",", $args));
}

sub skips
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing SKIP arguments, line $l, $f\n";
    exit 1;
  }
  $rcSkipRegex = &addRegEx($rcSkipRegex, $args);
}

sub priority
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing PRIORITY argument, line $l, $f\n";
    exit 1;
  }
  $args = lc($args);
  if (!&validatePriorityType($args)) {
    my ($lst) = &priorityTypeStr();
    print "invalid PRIORITY argument \"$args\", line $l, $f\nChoices for PRIORITY are: $lst\n";
    exit 1;
  }
  $rcPriority = $args;
}

sub strict
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing STRICT argument, line $l, $f\n";
    exit 1;
  }
  $args = lc($args);
  if (!&validateStrictType($args)) {
    my ($lst) = &strictTypeStr();
    print "invalid STRICT argument \"$args\", line $l, $f\nChoices for STRICT are: $lst\n";
    exit 1;
  }
  $rcStrict = $args;
}

sub output
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    my ($lst) = &outputTypeStr();
    print "missing OUTPUT argument, line $l, $f\nChoices for OUTPUT are: $lst\n";
    exit 1;
  }
  $args = lc($args);
  if (!&validateOutputType($args)) {
    print "invalid OUTPUT argument \"$args\", line $l, $f\n";
    exit 1;
  }
  $rcOutput = $args;
}

sub export
{
  my ($args, $l, $f) = @_;
  if (!defined($args)) {
    print "missing EXPORT argument, line $l, $f\n";
    exit 1;
  }
  $args = lc($args);
  if (!&validateExportType($args)) {
    my ($lst) = &exportTypeStr();
    print "invalid EXPORT argument \"$args\", line $l, $f\nChoices for EXPORT are: $lst\n";
    exit 1;
  }
  $rcExport = $args;
}

1;
