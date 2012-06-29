#!/usr/bin/perl -w

eval 'exec /usr/bin/perl -w -S $0 ${1+"$@"}'
    if 0; # not running under some shell
###############################################################################
# Copyright (C) 2007 Jos van den Oever <jos@vandenoever.info>
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

sub parseFile {
    my $file = shift;
    my $ininstall;
    my @files;
    open(FH, "< $file") or return;
    foreach(<FH>) {
        chomp;
	s/#.*$//;  #remove comments
        if (m/^\s*install\s*\(\s*files(\s+[^)]+)?(\s*\))?/i) {
            $ininstall = 1;
            if ($1) {
                push(@files, split('\s+', $1));
            }
            if ($2) {
                $ininstall = 0;
            }
        } elsif ($ininstall) {
            if (m/^\s*(.*\s)destination/i) {
                if ($1) {
                    push(@files, split('\s+', $1));
                }
                $ininstall = 0;
            } elsif (m/^\s*([^)]+)?\)/) {
                if ($1) {
                    push(@files, split('\s+', $1));
                }
                $ininstall = 0;
            } else {
                push(@files, split('\s+', $_));
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

