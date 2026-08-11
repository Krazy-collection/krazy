###############################################################################
# Sanity checks for your KDE source code                                      #
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>                  #
# SPDX-License-Identifier: GPL-2.0-or-later                                   #
###############################################################################

package Krazy::PreProcess;

use warnings;
use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);    ## no critic

use Exporter;
$VERSION = 1.00;
@ISA     = qw(Exporter);

@EXPORT    = qw(RemoveCommentsC RemoveIfZeroBlockC RemoveCondBlockC RemoveCommentsFDO);
@EXPORT_OK = qw();

# Replace C-style comments with whitespace in C/C++ source.
sub RemoveCommentsC
{

  my (@data_lines) = @_;

  #get all the c-style comments from the file
  my ($data)     = "@data_lines";
  my (@comments) = ($data =~ /\/\*.*?\*\//gs);

  #for each comment, remove everything but the linebreaks, so
  #our line numbering report does not get screwed up.
  foreach my $comment (@comments) {
    my ($fixed_comment) = $comment;
    $fixed_comment =~ s/[^\n]//gs;
    $fixed_comment =~ s/\n/\n/gs;
    $data          =~ s/\Q$comment/$fixed_comment/s;
  }

  #return array
  return split(/\n/, $data);
}

# Replace #if 0 blocks with whitespace in C/C++ source.
sub RemoveIfZeroBlockC
{

  my (@data_lines) = @_;
  my ($i)          = 0;
  my ($inblock)    = 0;
  while ($i < $#data_lines) {
    if ($inblock == 1) {
      if ($data_lines[$i] =~ m/^\s*#\s*endif\b/) {
        $inblock = 0;
      }
      $data_lines[$i++] = "\n";
      next;
    }
    if ($data_lines[$i] =~ m/^\s*#\s*if\s+0\b/) {
      $inblock = 1;
      $data_lines[$i++] = "\n";
      next;
    }
    $i++;
  }

  #return array
  return @data_lines;
}

# Replace //krazy:cond=checker blocks with whitespace in C++ source.
# Very stupid. Doesn't handle nested blocks. Doesn't handle C-style comments.
sub RemoveCondBlockC
{

  my ($checker, @data_lines) = @_;

  my ($i)       = 0;
  my ($inblock) = 0;
  while ($i < $#data_lines) {
    if ($inblock == 1) {
      if ($data_lines[$i] =~ m+//.*[Kk]razy:endcond=.*$checker+) {
        $inblock = 0;
      }
      $data_lines[$i++] = "\n";
      next;
    }
    if ($data_lines[$i] =~ m+//.*[Kk]razy:cond=.*$checker+) {
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
sub RemoveCommentsFDO
{

  my (@data_lines) = @_;

  my ($data)     = "@data_lines";
  my (@comments) = ($data =~ /^#.*$/gs);
  return @comments;
}

1;
