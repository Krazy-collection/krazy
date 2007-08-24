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

package Krazy::PreProcess;

use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);

use Exporter;
$VERSION = 1.00;
@ISA = qw(Exporter);

@EXPORT = qw(RemoveCommentsC RemoveIfZeroBlockC RemoveCommentsFDO);
@EXPORT_OK = qw();

# Replace C-style comments with whitespace in C/C++ source.
sub RemoveCommentsC {

  my(@data_lines) = @_;

  #get all the c-style comments from the file
  my($data)="@data_lines";
  my(@comments) = ($data =~ /\/\*.*?\*\//gs);

  #for each comment, remove everything but the linebreaks, so
  #our line numbering report does not get screwed up.
  foreach my $comment ( @comments ) {
    my($fixed_comment) = $comment;
    $fixed_comment =~ s/[^\n]//gs;
    $fixed_comment =~ s/\n/\n/gs;
    $data =~ s/\Q$comment/$fixed_comment/s;
  }

  #return array
  return split(/\n/, $data);
}

# Replace #if 0 blocks with whitespace in C/C++ source.
sub RemoveIfZeroBlockC {

  my(@data_lines) = @_;
  my($i) = 0;
  my($inblock) = 0;
  while ( $i < $#data_lines ) {
    if ( $inblock == 1 ) {
      if ( $data_lines[$i] =~ m/^\s*#\s*endif\b/ ) {
	$inblock = 0;
      }
      $data_lines[$i++] = "\n";
      next;
    }
    if ( $data_lines[$i] =~ m/^\s*#\s*if\s+0\b/ ) {
      $inblock = 1;
      $data_lines[$i++] = "\n";
      next;
    }
    $i++;
  }

  #return array
  return @data_lines;
}

# Replace comments with whitespace in .desktop source
sub RemoveCommentsFDO {

  my(@data_lines) = @_;

  my($data)="@data_lines";
  my(@comments) = ($data =~ /^#.*$/gs);
  return @comments;
}

1;
