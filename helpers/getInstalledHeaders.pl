#!/usr/bin/perl -w

eval 'exec /usr/bin/perl -w -S $0 ${1+"$@"}'
    if 0; # not running under some shell
###############################################################################
# Copyright (C) 2007 Jos van den Oever <jos@vandenoever.info>
# Copyright (C) 2012 Daniel Calviño Sánchez <danxuliu@gmail.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
###############################################################################

use strict;
use File::Basename;
use Cwd 'abs_path';

# Donated by "Jos van den Oever" <jos@vandenoever.info>, March 2007

# This script, called with a source directory as argument, prints all files
# that will be installed via the INSTALL(FILES ....) command of cmake.

my @allfiles;

# Returns the values of the variable.
# The values are got from the set command of the variable with the given name in
# the file to parse. Nothing fancy like resolving the variable from parent
# CMakeLists.txt or recursively expanding other variables referenced in the
# values is done.
sub getVariableValues {
    my $fileToParse = shift;
    my $variableName = shift;
    my $inVariableSet;
    my @values;

    open(FH, "< $fileToParse") or return;
    foreach(<FH>) {
        chomp;
        s/#.*$//;  #remove comments

        # Matches "set(VARIABLE_NAME"
        # Matches "set(VARIABLE_NAME value1 value2 value3..."
        # Matches "set(VARIABLE_NAME value1 value2 value3...)"
        #
        # The values can be separated one from the other with one or more
        # spaces.
        if (m/^\s*set\s*\(\s*$variableName(\s+[^)]+)?(\s*\))?/i) {
            $inVariableSet = 1;
            if ($1) {
                push(@values, split('\s+', $1));
            }
            if ($2) {
                $inVariableSet = 0;
            }
        } elsif ($inVariableSet) {
            # Matches ")"
            # Matches "value1 value2 value3... )"
            if (m/^\s*([^)]+)?\)/) {
                if ($1) {
                    push(@values, split('\s+', $1));
                }
                $inVariableSet = 0;
            } else {
                push(@values, split('\s+', $_));
            }
        }
    }

    return @values;
}

# Process the given fragment of the install command and returns the names of the
# files.
# If the fragment contains the name of a variable, the variable is expanded and
# the names of the files set in the variable are returned.
sub processFileNamesAndVariables {
    my $fileToParse = shift;
    my $fileNamesAndVariables = shift;
    my @fileNames;

    foreach (split('\s+', $fileNamesAndVariables)) {
        # Matches "${theNameOfTheVariable}"
        if (m/^\$\{(.*)\}/i && $1 !~ m/\$/) {
            push(@fileNames, getVariableValues($fileToParse, $1));
        } else {
            push(@fileNames, $_);
        }
    }

    return @fileNames;
}

sub parseFile {
    my $file = shift;
    my $inInstallFiles;
    my @files;
    open(FH, "< $file") or return;
    foreach(<FH>) {
        chomp;
        s/#.*$//;  #remove comments
        # Matches "install("
        #
        # Matches "install(FILES"
        #
        # Matches "install(FILES file1 file2 file3..."
        #
        # Matches "install(FILES file1 file2 file3... DESTINATION"
        #
        # The file names can even be something like "destination.h" or
        # "foodestination", and can be separated one from the other with one or
        # more spaces.
        # After DESTINATION more install arguments can appear, as long as they
        # are separated from DESTINATION with a space.
        # The matching is not case sensitive.
        if (m/^\s*install\s*\(\s*$/i ||
            m/^\s*install\s*\(\s*FILES\s*$/i ||
            m/^\s*install\s*\(\s*FILES\s+(((?:(?!\sDESTINATION(\s|$)).)*)+)$/i ||
            m/^\s*install\s*\(\s*FILES\s+(.*\s)+(DESTINATION(\s|$))/i) {
            $inInstallFiles = 1;
            if ($1) {
                push(@files, processFileNamesAndVariables($file, $1));
            }
            if ($2) {
                $inInstallFiles = 0;
            }
        } elsif ($inInstallFiles) {
            # Matches "DESTINATION"
            # Matches "file1 file2 file3... DESTINATION"
            #
            # The file names can even be something like "destination.h" or
            # "foodestination", and can be separated one from the other with one
            # or more spaces.
            # After DESTINATION more install arguments can appear, as long as
            # they are separated from DESTINATION with a space.
            # The matching is not case sensitive.
            if (m/^\s*(.*\s)?DESTINATION(\s|$)/i) {
                if ($1) {
                    push(@files, processFileNamesAndVariables($file, $1));
                }
                $inInstallFiles = 0;
            } else {
                push(@files, processFileNamesAndVariables($file, $_));
            }
        }
    }
    $file =~ s#[^/]*$##;

    foreach (@files) {
        my $path = $file.$_;
        if (length($_) && -f $path) {
            print "$file$_\n";
        }
    }
    close(FH);
}

if ($#ARGV != 0) {
    print STDERR "Usage $0 [directory]\n";
    exit 1;
}

#Compute the list of CMakeLists.txt files to search
# we look in the specified directory, and 1-level up
my(@makefiles);
my($absf) = abs_path($ARGV[0]);
push(@makefiles, $absf . "/CMakeLists.txt");
push(@makefiles, dirname($absf) . "/CMakeLists.txt");

foreach (@makefiles) {
    chomp;
    parseFile($_);
}
