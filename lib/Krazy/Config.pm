###############################################################################
# Sanity checks for your KDE source code                                      #
# Copyright 2007 by Allen Winter <winter@kde.org>                             #
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

use Exporter;
$VERSION = 1.00;
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
# SKIP regexp
# IGNORESUBS subdir1[,subdir2,...]
# IGNOREMODS module1[,module2,...]
#
# Multiple directives may be specified per file; they will be combined in
# a logical way.
#
# The directive is case-insensitive.
#==============================================================================

my($rcExclude,$rcOnly,$rcExtra,$rcSkipRegex,@rcIgSubsList,@rcIgModsList);
my($CWD);

sub ParseKrazyRC {
  my ($rcfile) = @_;
  open( F, "$rcfile" ) || return;

  my ( $line, $linecnt, $directive, $arg );
  $CWD = getcwd;

  $rcExclude    = "";
  $rcOnly       = "";
  $rcExtra      = "";
  $rcSkipRegex  = "";
  @rcIgSubsList = ();
  @rcIgModsList = ();

  while ( $line = <F> ) {
    $linecnt++;
    $line =~ s/#.*//;     #strip comment
    $line =~ s/^\s+//;    #strip leading whitespace
    $line =~ s/\s+$//;    #strip trailing whitespace
    next if ( !$line );

    ( $directive, $arg ) = split( " ", $line );
    $directive = uc($directive);
    if ( $directive eq "EXTRA" ) {
      &extras($arg);
    } elsif ( $directive eq "CHECK" ) {
      &checks($arg);
    } elsif ( $directive eq "EXCLUDE" ) {
      &excludes($arg);
    } elsif ( $directive eq "IGNORESUBS" ) {
      &ignoreSubs($arg);
    } elsif ( $directive eq "IGNOREMODS" ) {
      &ignoreMods($arg);
    } elsif ( $directive eq "SKIP" ) {
      &skips($arg);
    } else {
      print "Invalid directive $directive, line $linecnt, $rcfile... exiting\n";
      close(F);
      exit 1;
    }
  }
  close(F);

  #return a hash of the directives
  my(%directives);
  $directives{'EXCLUDE'}    = $rcExclude;
  $directives{'CHECK'}      = $rcOnly;
  $directives{'EXTRA'}      = $rcExtra;
  $directives{'SKIPREGEX'}  = $rcSkipRegex;
  @{$directives{'IGSUBSLIST'}} = deDupe(@rcIgSubsList);
  @{$directives{'IGMODSLIST'}} = deDupe(@rcIgModsList);
  return %directives;
}

sub extras {
  my ($args) = @_;  
  if ( !defined($args) ) {
    print "missing EXTRA arguments in .krazy...exiting\n";
    exit 1;
  }
  if ( !$rcExtra ) {
    $rcExtra = $args;
  } else {
    $rcExtra .= "," . $args;
  }
}

sub checks {
  my ($args) = @_;
  if ( !defined($args) ) {
    print "missing CHECK arguments in .krazy...exiting\n";
    exit 1;
  }
  if ( !$rcOnly ) {
    $rcOnly = $args;
  } else {
    $rcOnly .= "," . $args;
  }
}

sub excludes {
  my ($args) = @_;
  if ( !defined($args) ) {
    print "missing EXCLUDE arguments in .krazy...exiting\n";
    exit 1;
  }
  if ( !$rcExclude ) {
    $rcExclude = $args;
  } else {
    $rcExclude .= "," . $args;
  }
}

sub ignoreSubs {
  my ($args) = @_;
  if ( !defined($args) ) {
    print "missing IGNORESUBS arguments in .krazy...exiting\n";
    exit 1;
  }
  push( @rcIgSubsList, split( ",", $args ) );
}

sub ignoreMods {
  my ($args) = @_;
  if ( !defined($args) ) {
    print "missing IGNOREMODS arguments in .krazy...exiting\n";
    exit 1;
  }
  push( @rcIgModsList, split( ",", $args ) );
}

sub skips {
  my ($args) = @_;
  if ( !defined($args) ) {
    print "missing SKIP arguments in .krazy...exiting\n";
    exit 1;
  }
  $args =~ s+\\\|+|+g;
  return if (!length($args));
  if ( !$rcSkipRegex ) {
    $rcSkipRegex = $args;
  } else {
    $rcSkipRegex .= "|" . $args;
  }
}

1;
