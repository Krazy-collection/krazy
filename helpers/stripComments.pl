#!/usr/bin/perl -n
#
# SPDX-FileCopyrightText: 2006 Thomas Zander <zander@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

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
