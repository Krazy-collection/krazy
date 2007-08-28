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

package Krazy::Utils;

use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);
use Cwd 'abs_path';

use Exporter;
$VERSION = 1.00;
@ISA = qw(Exporter);

@EXPORT = qw(topModule topSubdir deDupe);
@EXPORT_OK = qw();

my($ModRegex) = "(kde(libs|pimlibs|base|accessibility|addons|admin|artwork|bindings|edu|games|graphics|multimedia|network|pim|sdk|toys|utils|velop|vplatform|webdev|support|review)|extragear|playground|koffice)";

#full path to the top of the module where the specified file resides
sub topModule {
  my($in) = @_;
  my($apath) = abs_path($in);
  my($top) = $apath;
  $top =~ s+/$ModRegex/.*++;
  return "" if ( $top eq $apath );
  my($module) = $apath;
  $module =~ s+$top/++;
  $module =~ s+/.*++;
  return "$top/$module";
}

#full path to the top of the module/subdir where the specified file resides
sub topSubdir {
  my($in) = @_;
  my($apath) = abs_path($in);
  my($top) = $apath;
  $top =~ s+/$ModRegex/.*++;
  return "" if ( $top eq $apath );
  my($module) = $apath;
  $module =~ s+$top/++;
  return "" if ( $module eq $top ); # not in a KDE module
  my($subdir);
  ($module,$subdir) = split("/",$module);
  return "$top/$module/$subdir";
}

# remove duplicate entries from a list
sub deDupe {
  my (@list) = @_;
  my (%seen) = ();
  my ( @uniq, $item );
  foreach $item (@list) {
    push( @uniq, $item ) unless $seen{$item}++;
  }
  @list = @uniq;
}

1;
