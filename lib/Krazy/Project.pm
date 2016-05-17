###############################################################################
# Sanity checks for your source code                                          #
# Copyright 2016 by Allen Winter <winter@kde.org>                             #
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

package Krazy::Project;

use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);
use Cwd;
use Cwd 'abs_path';
use File::Basename;

use Exporter;
$VERSION = 0.95;
@ISA = qw(Exporter);

@EXPORT = qw(topOfProject projectType);
@EXPORT_OK = qw();

my($PROJECT_TYPE) = "";
sub setProjectType {
  ($PROJECT_TYPE) = @_;
}
sub projectType {
  return $PROJECT_TYPE;
}

#search for any one of the strings in specified file.
# case is ignored. lines with leading whitespace are merged onto previous line
#returns:
# the line number (>0) of the first match
# 0 if there is no match
# -1 if the file cannot be opened for reading
sub linesCaseSearchInFile {
  my($f, @lines) = @_;

  my($cnt) = -1;
  if (!open(F, "$f")) {
    return $cnt;
  }
  $cnt++;
  while (<F>) {
    $cnt++;
    chomp($_);
    my($l);
    for $l (@lines) {
      if ($_ =~ m/$l/i) {
        close(F);
        return $cnt;
      }
    }
  }
  close(F);
  return 0;
}

# findUp: from $1, searches up no farther than the dir named in $2
sub findUp {
  my($td, $d) = @_;
  chdir($td) || return "";
  while (! -e $d) {
    if (!chdir("..")) {
      return "";
    }
    my($pwd) = getcwd();
    if ($pwd eq "/") {
      return "";
    }
  }
  return getcwd();
}

# findUpCMakeProject: from $1, searches up no farther than the dir named in $2 for a CMakeLists.txt file with a project line
sub findUpCMakeProject {
  my($td, $d) = @_;
  my($f) = "CMakeLists.txt";
  chdir($td) || return "";
  while (! -e $d) {
    last if (-e $f && (&linesCaseSearchInFile($f, ("project\\s*\\(\\s*")) > 0));
    if (!chdir("..")) {
      return "";
    }
    my($pwd) = getcwd();
    if ($pwd eq "/") {
      return "";
    }
  }
  return "" if (! -e $f);
  return getcwd() . "/" . $f;
}

# findUpProProject: from $1, searches up no farther than the dir named in $2 for a foo.pro file in dir foo
sub findUpProProject {
  my($td, $d) = @_;
  chdir($td) || return "";
  while (! -e $d) {
    if (!chdir("..")) {
      return "";
    }
    my($pwd) = getcwd();
    if ($pwd eq "/") {
      return "";
    }
  }

  my($f);
  $f = basename(getcwd()) . ".pro";
  return "" if (! -e $f);
  return getcwd() . "/" . $f;
}

# findUp: from $1, searches up for the dir named in $2 and see if a .pro file lives in there
sub findUpPro {
  my($td, $d) = @_;
  chdir($td) || return "";
  while (! -e $d) {
    if (!chdir("..")) {
      return "";
    }
    my($pwd) = getcwd();
    if ($pwd eq "/") {
      return "";
    }
  }
  my(@matches) = glob("*.pro");
  return getcwd() . "/.pro" if ($#matches >= 0);
  return "";
}

# findUpConfigure: from $1, searches up no farther than the dir named in $2 for a configure file
sub findUpConfigure {
  my($td, $d) = @_;
  my($f1) = "configure";
  my($f2) = "configure.sh";
  chdir($td) || return "";
  while (! -e $d) {
    last if (-x $f1 || -x $f2);
    if (!chdir("..")) {
      return "";
    }
    my($pwd) = getcwd();
    if ($pwd eq "/") {
      return "";
    }
  }
  return "" if (! -x $f1 && ! -x $f2);
  return getcwd() . "/" . $f1;
}

# topOfProject: use various heuristics to find the top-level path of the project from $CWD
sub topOfProject {
  my($td) = @_;
  return "" if (!defined($td));
  return "" if (! -e "$td");

  my(@scms) = (".git", ".hg", ".svn");
  my($s);

  my($t) = "";

  my($SAVEPATH) = getcwd;
  setProjectType("");

  #look up for a CMakeLists.txt with a project line (stopping at SCM level)
  for $s (@scms) {
    if (! $t) {
      $t = findUpCMakeProject($td, $s);
      if ($t) {
        &setProjectType("CMake");
        goto Done;
      }
    }
  }

  # look up for a project.pro file in a dir called project (stopping at SCM level)
  for $s (@scms) {
    if (! $t) {
      $t = findUpProProject($td, $s);
      if ($t) {
        &setProjectType("QMake");
        goto Done;
      }
    }
  }

  # look up for any .pro file in the specified dir (stopping at SCM level)
  for $s (@scms) {
    if (! $t) {
      $t = findUpPro($td, $s);
      if ($t) {
        &setProjectType("QMake unnamed .pro");
        goto Done;
      }
    }
  }

  # look up for a configure file (stopping at SCM level)
  for $s (@scms) {
    if (! $t) {
      $t = findUpConfigure($td, $s);
      if ($t) {
        &setProjectType("autoconf");
        goto Done;
      }
    }
  }

  # look up for an SCM
  for $s (@scms) {
    if (! $t) {
      $t = findUp($td, $s);
      if ($t) {
        &setProjectType("top of SCM");
        goto Done;
      }
    }
  }

 Done:
  chdir($SAVEPATH);
  return dirname($t) if ($t);
  return "";
}

1;
