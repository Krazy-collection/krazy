###############################################################################
# Sanity checks for your KDE source code                                      #
# Copyright 2007,2009-2021 by Allen Winter <winter@kde.org>                   #
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

#TODO:
# handle second args in EXCLUDE, CHECK, EXTRA directives

package Krazy::Config;

use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);
use Cwd;
use Krazy::Utils;
use Env qw (KRAZY_STYLE_CPPSTYLE KRAZY_STYLE_OFFSET KRAZY_STYLE_LINEMAX);

use Exporter;
$VERSION = 1.60;
@ISA = qw(Exporter);

@EXPORT = qw(ParseKrazyRC);
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
# EXPORT <text|textlist|textedit|xml>
# IGNORESUBS subdir1[,subdir2,...]
# EXTRASUBS subdir1[,subdir2,...]
# IGNOREMODS module1[,module2,...]
# STYLE_CPPSTYLE <qt|kde|pim>
# STYLE_OFFSET <integer > 0>
# STYLE_LINEMAX <integer > 0>
# CPP_INCLUDE_ORDER <true|false>
#
# Multiple directives may be specified per file; they will be combined in
# a logical way.
#
# The directive is case-insensitive.
#==============================================================================

my($rcExclude,$rcOnly,$rcCheckSets,$rcExtra,$rcIncTypes,$rcExcTypes,
   $rcSkipRegex,$rcPriority,$rcStrict,$rcOutput,$rcExport);
my(@rcIgSubsList,@rcExSubsList,@rcIgModsList);
my($CWD);

sub ParseKrazyRC {
  my ($rcfile) = @_;

  my(%directives);
  open( F, "$rcfile" ) || return %directives;

  my ( $line, $linecnt, $directive, $arg );
  $CWD = getcwd;

  $rcExclude    = "";
  $rcOnly       = "";
  $rcCheckSets  = "";
  $rcExtra      = "";
  $rcIncTypes   = "";
  $rcExcTypes   = "";
  $rcSkipRegex  = "";
  $rcPriority   = "";
  $rcStrict     = "";
  $rcOutput     = "";
  $rcExport     = "";
  @rcIgSubsList = ();
  @rcExSubsList = ();
  @rcIgModsList = ();
  $ENV{KRAZY_CPP_INCLUDE_ORDER} = "false";

  while ( $line = <F> ) {
    $linecnt++;
    $line =~ s/#.*//;       #strip comment
    $line =~ s/^\s+//;      #strip leading whitespace
    $line =~ s/\s+$//;      #strip trailing whitespace
    $line =~ s/\s*,\s*/,/g; #remove whitespace around commas
    next if ( !$line );

    ( $directive, $arg ) = split( " ", $line );
    my $saveDirective = $directive;
    $directive = uc($directive);
    if ( $directive eq "EXTRA" ) {
      &extras($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "CHECK" ) {
      &checks($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "CHECKSETS" ) {
      &checkSets($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "EXCLUDE" ) {
      &excludes($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "IGNORESUBS" ) {
      &ignoreSubs($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "EXTRASUBS" ) {
      &extraSubs($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "IGNOREMODS" ) {
      &ignoreMods($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "SKIP" ) {
      &skips($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "PRIORITY" ) {
      &priority($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "STRICT" ) {
      &strict($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "OUTPUT" ) {
      &output($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "EXPORT" ) {
      &export($arg, $linecnt, $rcfile);
    } elsif ( $directive eq "TYPES" ) {
      &types($arg, $linecnt, $rcfile );
    } elsif ( $directive eq "EXCLUDETYPES" ) {
      &excludeTypes($arg, $linecnt, $rcfile );
    } elsif ( $directive eq "STYLE_CPPSTYLE" ||
              $directive eq "STYLE_OFFSET" ||
              $directive eq "STYLE_LINEMAX") {
      &styleSettings( $directive, $arg, $linecnt, $rcfile );
    } elsif ( $directive eq "STYLE_CMAKESTYLE_STYLE" ||
              $directive eq "STYLE_CMAKESTYLE_OFFSET" ||
              $directive eq "STYLE_CMAKESTYLE_LINEMAX") {
      &cmakeStyleSettings( $directive, $arg, $linecnt, $rcfile );
    } elsif ( $directive eq "STYLE_PYTHONSTYLE_STYLE" ||
              $directive eq "STYLE_PYTHONSTYLE_OFFSET" ||
              $directive eq "STYLE_PYTHONSTYLE_LINEMAX") {
      &pythonStyleSettings( $directive, $arg, $linecnt, $rcfile );
    } elsif ( $directive eq "CPP_INCLUDE_ORDER") {
      &cppIncludeSettings( $directive, $arg, $linecnt, $rcfile );
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
  @{$directives{'IGSUBSLIST'}} = deDupe(@rcIgSubsList);
  @{$directives{'IGEXTRASLIST'}} = deDupe(@rcExSubsList);
  @{$directives{'IGMODSLIST'}} = deDupe(@rcIgModsList);
  return %directives;
}

sub extras {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing EXTRA arguments, line $l, $f\n";
    exit 1;
  }
  if ( !$rcExtra ) {
    $rcExtra = $args;
  } else {
    $rcExtra .= "," . $args;
  }
}

sub checks {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing CHECK arguments, line $l, $f\n";
    exit 1;
  }
  if ( !$rcOnly ) {
    $rcOnly = $args;
  } else {
    $rcOnly .= "," . $args;
  }
}

sub checkSets {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing CHECKSETS arguments, line $l, $f\n";
    exit 1;
  }
  if ( !$rcCheckSets ) {
    $rcCheckSets = $args;
  } else {
    $rcCheckSets .= "," . $args;
  }
}

sub excludes {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing EXCLUDE arguments, line $l, $f\n";
    exit 1;
  }
  if ( !$rcExclude ) {
    $rcExclude = $args;
  } else {
    $rcExclude .= "," . $args;
  }
}

sub types {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing TYPES arguments, line $l, $f\n";
    exit 1;
  }
  if ( !$rcIncTypes ) {
    $rcIncTypes = $args;
  } else {
    $rcIncTypes .= "," . $args;
  }
}

sub excludeTypes {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing EXCLUDETYPES arguments, line $l, $f\n";
    exit 1;
  }
  if ( !$rcExcTypes ) {
    $rcExcTypes = $args;
  } else {
    $rcExcTypes .= "," . $args;
  }
}

sub styleSettings {
  my ($s, $args, $l, $f) = @_;
  if ( !defined($args) ) {
    print "missing $s arguments, line $l, $f\n";
    exit 1;
  }

  if ($s eq "STYLE_CPPSTYLE") {
    if ( !&validateStyleType($args) ) {
      my($lst) = &styleTypeStr();
      print "invalid STYLE_CPPSTYLE argument \"$args\", line $l, $f\nChoices are: $lst\n";
      exit 1;
    } else {
      $ENV{KRAZY_STYLE_CPPSTYLE} = $args;
    }
  } elsif ($s eq "STYLE_OFFSET") {
    my ($offset) = sprintf("%d", $args);
    if ($offset < 1) {
      print "setting $s value less than 1, line $l, $f\n";
      exit 1;
    } else {
      $ENV{KRAZY_STYLE_OFFSET} = $offset;
    }
  } elsif ($s eq "STYLE_LINEMAX") {
    my ($max) = sprintf("%d",$args);
    if ($max < 0) {
      print "setting $s value less than 0, line $l, $f\n";
      exit 1;
    } else {
      $ENV{KRAZY_STYLE_LINEMAX} = $max;
    }
  } else {
    print "unknown style setting $s, line $l, $f\n";
    exit 1;
  }
}

sub cmakeStyleSettings {
  my ($s, $args, $l, $f) = @_;
  if ( !defined($args) ) {
    print "missing $s arguments, line $l, $f\n";
    exit 1;
  }

  if ($s eq "STYLE_CMAKESTYLE_STYLE") {
    if ( !&validateCMakeStyleType($args) ) {
      my($lst) = &cmakeStyleTypeStr();
      print "invalid STYLE_CMAKESTYLE_STYLE argument \"$args\", line $l, $f\nChoices are: $lst\n";
      exit 1;
    } else {
      $ENV{KRAZY_CMAKESTYLE_STYLE} = $args;
    }
  } elsif ($s eq "STYLE_CMAKESTYLE_OFFSET") {
    my ($offset) = sprintf("%d", $args);
    if ($offset < 1) {
      print "setting $s value less than 1, line $l, $f\n";
      exit 1;
    } else {
      $ENV{KRAZY_CMAKESTYLE_OFFSET} = $offset;
    }
  } elsif ($s eq "STYLE_CMAKESTYLE_LINEMAX") {
    my ($max) = sprintf("%d",$args);
    if ($max < 0) {
      print "setting $s value less than 0, line $l, $f\n";
      exit 1;
    } else {
      $ENV{KRAZY_CMAKESTYLE_LINEMAX} = $max;
    }
  } else {
    print "unknown style setting $s, line $l, $f\n";
    exit 1;
  }
}

sub pythonStyleSettings {
  my ($s, $args, $l, $f) = @_;
  if ( !defined($args) ) {
    print "missing $s arguments, line $l, $f\n";
    exit 1;
  }

  if ($s eq "STYLE_PYTHONSTYLE_STYLE") {
    if ( !&validatePythonStyleType($args) ) {
      my($lst) = &pythonStyleTypeStr();
      print "invalid STYLE_PYTHONSTYLE_STYLE argument \"$args\", line $l, $f\nChoices are: $lst\n";
      exit 1;
    } else {
      $ENV{KRAZY_PYTHONSTYLE_STYLE} = $args;
    }
  } elsif ($s eq "STYLE_PYTHONSTYLE_OFFSET") {
    my ($offset) = sprintf("%d", $args);
    if ($offset < 1) {
      print "setting $s value less than 1, line $l, $f\n";
      exit 1;
    } else {
      $ENV{KRAZY_PYTHONSTYLE_OFFSET} = $offset;
    }
  } elsif ($s eq "STYLE_PYTHONSTYLE_LINEMAX") {
    my ($max) = sprintf("%d",$args);
    if ($max < 0) {
      print "setting $s value less than 0, line $l, $f\n";
      exit 1;
    } else {
      $ENV{KRAZY_PYTHONSTYLE_LINEMAX} = $max;
    }
  } else {
    print "unknown style setting $s, line $l, $f\n";
    exit 1;
  }
}

sub cppIncludeSettings {
  my ($s, $args, $l, $f) = @_;
  if ( !defined($args) ) {
    print "missing $s arguments, line $l, $f\n";
    exit 1;
  }

  if ($s eq "CPP_INCLUDE_ORDER") {
    if ( !&validateCppIncludeOrderType($args) ) {
      my($lst) = &cppIncludeOrderTypeStr();
      print "invalid CPP_INCLUDE_ORDER argument \"$args\", line $l, $f\nChoices are: $lst\n";
      exit 1;
    } else {
      $args = lc($args);
      if ($args eq "yes" || $args eq "on" || $args eq "true" ) {
        $ENV{KRAZY_CPP_INCLUDE_ORDER} = "true";
      } else {
        $ENV{KRAZY_CPP_INCLUDE_ORDER} = "false";
      }
    }
  }
}

sub ignoreSubs {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing IGNORESUBS arguments, line $l, $f\n";
    exit 1;
  }
  push( @rcIgSubsList, split( ",", $args ) );
}

sub extraSubs {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing EXTRASUBS arguments, line $l, $f\n";
    exit 1;
  }
  push( @rcExSubsList, split( ",", $args ) );
}

sub ignoreMods {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing IGNOREMODS arguments, line $l, $f\n";
    exit 1;
  }
  push( @rcIgModsList, split( ",", $args ) );
}

sub skips {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing SKIP arguments, line $l, $f\n";
    exit 1;
  }
  $rcSkipRegex = &addRegEx($rcSkipRegex,$args);
}

sub priority {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing PRIORITY argument, line $l, $f\n";
    exit 1;
  }
  $args=lc($args);
  if ( !&validatePriorityType($args) ) {
    my($lst) = &priorityTypeStr();
    print "invalid PRIORITY argument \"$args\", line $l, $f\nChoices for PRIORITY are: $lst\n";
    exit 1;
  }
  $rcPriority = $args;
}

sub strict {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing STRICT argument, line $l, $f\n";
    exit 1;
  }
  $args=lc($args);
  if ( !&validateStrictType($args) ) {
    my($lst) = &strictTypeStr();
    print "invalid STRICT argument \"$args\", line $l, $f\nChoices for STRICT are: $lst\n";
    exit 1;
  }
  $rcStrict = $args;
}

sub output {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    my($lst) = &outputTypeStr();
    print "missing OUTPUT argument, line $l, $f\nChoices for OUTPUT are: $lst\n";
    exit 1;
  }
  $args=lc($args);
  if ( !&validateOutputType($args) ) {
    print "invalid OUTPUT argument \"$args\", line $l, $f\n";
    exit 1;
  }
  $rcOutput = $args;
}

sub export {
  my ($args,$l,$f) = @_;
  if ( !defined($args) ) {
    print "missing EXPORT argument, line $l, $f\n";
    exit 1;
  }
  $args=lc($args);
  if ( !&validateExportType($args) ) {
    my($lst) = &exportTypeStr();
    print "invalid EXPORT argument \"$args\", line $l, $f\nChoices for EXPORT are: $lst\n";
    exit 1;
  }
  $rcExport = $args;
}

1;
