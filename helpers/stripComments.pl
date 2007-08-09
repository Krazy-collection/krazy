#!/usr/bin/perl -n
# Copyright (C) 2006 Thomas Zander <zander@kde.org>
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

# remove all comments
$_=~s/\/\/.*$//; # strip comments of a // stile comment.
if($inIfNull == 1) {
    if(/^\s*#\s*endif\b/) {
        $inIfNull = 0;
        print "\n";
        next;
    }
    else {
        print "\n";
        next;
    }
}
if($inComment == 1) {
    if(/^(.*?)\*\//) {
        $_=~s/.*?\*\///;
        $inComment = 0;
    }
    else {
        print "\n";
        next;
    }
}
if(/\/\*(.*?)(\*\/|$)/) {
    my $comment = $1;
    my $end = $2;
    if($end eq "*/") {
        $_=~s/\/\*.*?\*\///; # remove it.
    }
    else {
        $inComment=1;
        $_=~s/\/\*.*?$//;
    }
}
if(/^\s*#\s*if\s+0\b/) {
    $inIfNull=1;
    print "\n";
    next;
}

print "$_";
