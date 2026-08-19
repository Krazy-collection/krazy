###############################################################################
# Sanity checks for your source code                                          #
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>                  #
# SPDX-License-Identifier: GPL-2.0-or-later                                   #
###############################################################################

package Krazy::Utils;

use warnings;
use strict;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);    ## no critic
use Cwd;
use Cwd 'abs_path';
use POSIX qw (setlocale strftime LC_TIME);
use File::Basename;
use File::Glob ':bsd_glob';
use File::Find;
use File::Spec::Functions 'catfile';
use Getopt::Long;

use Exporter;
$VERSION = 2.99999;                                            # this is the module version
@ISA     = qw(Exporter);

@EXPORT = qw(topOfProject
  userMessage userError Exit
  fileType validateFileType fileTypeIs findFiles findFileByRegex asOf deDupe addRegEx
  addCommaSeparated commaSeparatedToArray arrayToCommaSeparated
  parseArgs helpArg versionArg priorityArg strictArg
  explainArg quietArg verboseArg installedArg
  priorityTypeStr strictTypeStr exportTypeStr
  outputTypeStr checksetTypeStr
  cppIncludeOrderTypeStr
  validateExportType validatePriorityType validateStrictType
  validateOutputType validateCheckSet validateCheckSets dedeprecateCheckSets dedeprecateCheckSetsStr
  validateCppIncludeOrderType
  usingCheckSet usingQtCheckSet usingKDECheckSet
  linesSearchInFile linesCaseSearchInFile
  allLinesCaseSearchInFile
  guessCheckSet checkSetDesc checkSetsList prettyPrintCheckSetsList
  fileTypeDesc fileTypesList prettyPrintTypesList
  printIssue printIssueTextEdit
  isCInclude isCSource);
@EXPORT_OK = qw();

my (@tmp);
my (@Exports) = (
  "text",        # text report, issues grouped by file
  "textlist",    # plain old text, 1 offending file-per-line
  "textedit",    # text formatted for IDEs, 1 issue-per-line
  "gitlab",      # JSON formatted for GitLab reporting
);

my (@Priorities) = (
  "all",          # low+normal+high
  "low",          # low only
  "normal",       # normal only
  "important",    # low+normal
  "high"          # high only
);

my (@Stricts) = (
  "all",          # super+normal
  "super",        # super only
  "normal"        # normal only
);

my (@Outputs) = (
  "normal",       # standard stuff (default)
  "brief",        # only checks with issues
  "quiet"         # no output, even if verbosity turned-on
);

my (@CppIncludeOrderTypes) = ("true", "false", "yes", "no", "on", "off");

my (@FileTypes) =
  ('c++', 'cmake', 'desktop', 'designer', 'kconfigxt', 'kpartgui', 'qml', 'qdoc', 'perl', 'python', 'json', 'svg');

my (@Sets) = (
  "c++",          # Pure C/C++ source
  "qt",           # Qt source (C++, QML, Qt Designer, Qt Doc)
  "kde",          # KDE source (C++, Qt, FDO desktop files, etc.)
  "kde-ci",       # KDE specific to the KDE CI (C++, Qt Designer, FDO desktop files, etc.)
  "foss"          # Free and open source software (FOSS)
);

#full path to the top of the project dir where the specified file resides
sub topOfProject
{
  # TODO: only supports git
  my ($top) = `git rev-parse --show-toplevel`;
  chomp($top);
  return $top;
}

# Exit a checker with the number of issues
sub Exit
{
  my ($issues) = @_;
  if ($issues > 0) {
    print "ISSUES=$issues\n";
  }
  exit $issues;
}

# Print a user message, if not quiet
sub userMessage
{
  my ($message, $quiet) = @_;
  if ($message && !$quiet) {
    print STDERR $message;
    print STDERR "\n";
  }
}

# Print a user error message and exit
sub userError
{
  my ($message) = @_;
  &userMessage($message);
  exit 1;
}

# remove duplicate entries from a list
sub deDupe
{
  my (@list) = @_;
  my (%seen) = ();
  my (@unik);
  foreach my ($item) (@list) {
    push(@unik, $item) unless $seen{$item}++;
  }
  return @unik;
}

sub addRegEx
{
  my ($r1, $r2) = @_;

  if ($r1) {
    if ($r2) {
      $r1 .= "|" . $r2;
    }
  } else {
    $r1 = $r2;
  }
  return $r1;
}

sub addCommaSeparated
{
  my ($l1, $l2) = @_;

  $l1 =~ s/\s*//g;
  $l2 =~ s/\s*//g;    # remove whitespace
  $l1 =~ s/^,+//;
  $l2 =~ s/^,+//;     # remove leading commas
  $l1 =~ s/,+$//;
  $l2 =~ s/,+$//;     # remove trailing commas

  if ($l1) {
    if ($l2) {
      $l1 .= "," . $l2;
    }
  } else {
    $l1 = $l2;
  }
  return &arrayToCommaSeparated(&deDupe(&commaSeparatedToArray($l1)));
}

sub commaSeparatedToArray
{
  my ($l) = @_;

  $l =~ s/\s*//g;    # remove whitespace
  $l =~ s/^,+//;     # remove leading commas
  $l =~ s/,+$//;     # remove trailing commas
  return split(",", $l);
}

sub arrayToCommaSeparated
{
  my (@a) = @_;
  my ($l) = "";
  for my ($guy) (@a) {
    $l .= $guy . ",";
  }
  $l =~ s/,$//;
  return $l;
}

# print a pretty list of supported file types with descriptions
sub prettyPrintTypesList()
{
  foreach my ($type) (&fileTypesList()) {
    printf("%12.12s: %s\n", $type, &fileTypeDesc($type));
  }
}

# returns 1 if the specified file type is valid
sub validateFileType()
{
  my ($type) = @_;
  if ($type) {
    $type = lc($type);
    return grep {$_ eq $type} @FileTypes;
  }
  return 0;
}

# returns an array of supported file types
sub fileTypesList()
{
  return @FileTypes;
}

# return a file type string determined by the filename extension
# returns an empty string if an unsupported file type is encountered.
sub fileType
{
  my ($f) = @_;

  if ($f =~ m/\.(?:cpp|cc|cxx|mm|c|C|tcc|h|hxx|hpp|H\+\+)$/) {
    return "c++";
  } elsif ($f =~ m/\.(?:cpp|cc|cxx|mm|c|C|tcc|h|hxx|hpp|H\+\+)\.in$/) {
    return "c++";
  } elsif ($f =~ m/\.(?:cpp|cc|cxx|mm|c|C|tcc|h|hxx|hpp|H\+\+)\.cmake$/) {
    return "c++";
  } elsif ($f =~ m/\.desktop$/) {
    return "desktop";
  } elsif ($f =~ m/\.ui$/) {
    return "designer";
  } elsif ($f =~ m/\.kcfg$/) {
    return "kconfigxt";
  } elsif ($f =~ m/\.po$/) {
    return "po";
  } elsif ($f =~ m/\.rc$/) {
    return "kpartgui";
  } elsif ($f =~ m/\.qml$/) {
    return "qml";
  } elsif ($f =~ m/\.qdoc$/) {
    return "qdoc";
  } elsif ($f =~ m/CMakeLists\.txt$/ || $f =~ m/\.cmake$/) {
    if ($f =~ m/\.kcfg\.cmake$/) {
      return "kconfigxt";
    } elsif ($f =~ m/\.py\.cmake$/) {
      return "python";
    } elsif ($f =~ m/\.desktop\.cmake$/) {
      return "desktop";
    } elsif ($f =~ m/\.json\.cmake$/) {
      return "json";
    }
    return "cmake";
  } elsif ($f =~ m/\.cmake\.in$/) {
    return "cmake";
  } elsif ($f =~ m/\.pl$/i && $f !~ m/makefile\.pl$/i) {
    return "perl";
  } elsif ($f =~ m/\.pm$/i) {
    return "perl";
  } elsif ($f =~ m/\.py$/) {
    return "python";
  } elsif ($f =~ m/\.json$/i) {
    return "json";
  } elsif ($f =~ m/\.svg$/) {
    return "svg";
  }
  return "";
}

sub fileTypeDesc
{
  my ($t) = @_;

  if ($t eq "c++") {
    return "C/C++ source code";
  } elsif ($t eq "desktop") {
    return "Freedesktop.org desktop files";
  } elsif ($t eq "designer") {
    return "Qt Designer files";
  } elsif ($t eq "kconfigxt") {
    return "KConfigXT XML files";
  } elsif ($t eq "po") {
    return "GNU gettext files";
  } elsif ($t eq "kpartgui") {
    return "KPartGUI files";
  } elsif ($t eq "qml") {
    return "Qt Declarative UI files";
  } elsif ($t eq "qdoc") {
    return "Qt documentation files";
  } elsif ($t eq "cmake") {
    return "CMake files";
  } elsif ($t eq "perl") {
    return "Perl program files";
  } elsif ($t eq "python") {
    return "Python program files";
  } elsif ($t eq "json") {
    return "JSON files";
  } elsif ($t eq "svg") {
    return "SVG (uncompressed) files";
  }
  return "";
}

#is the file of the type that is specified?
sub fileTypeIs
{
  my ($f, $t) = @_;

  #special case C vs. C++
  if ($t eq "c++") {
    return 1 if ($f =~ m/\.(?:cpp|cc|cxx|mm|C|tcc|hxx|hpp|H\+\+)$/);
    return 1 if ($f =~ m/\.(?:cpp|cc|cxx|mm|C|tcc|hxx|hpp|H\+\+)\.in$/);
    return 1 if ($f =~ m/\.(?:cpp|cc|cxx|mm|C|tcc|hxx|hpp|H\+\+)\.cmake$/);
    if ($f =~ m/\.h$/) {
      my ($tf) = $f;
      $tf =~ s/\.h$/\.cpp/;
      return 1 if (-e $tf);
      $tf =~ s/\.cpp$/\.cc/;
      return 1 if (-e $tf);
      $tf =~ s/\.cc$/\.cxx/;
      return 1 if (-e $tf);
      $tf =~ s/\.cxx$/\.tcc/;
      return 1 if (-e $tf);
      $tf =~ s/\.tcc$/\.mm/;
      return 1 if (-e $tf);

      #causes insanity on case-insensitive filesystems
      #$tf =~ s/\.mm$/\.C/;
      #return 1 if (-e $tf);
    }
    if ($f =~ m/\.h\.in$/) {
      my ($tf) = $f;
      $tf =~ s/\.h\.in$/\.cpp\.in/;
      return 1 if (-e $tf);
      $tf =~ s/\.cpp\.in$/\.cc.in/;
      return 1 if (-e $tf);
      $tf =~ s/\.cc\.in$/\.cxx.in/;
      return 1 if (-e $tf);
      $tf =~ s/\.cxx\.in$/\.tcc.in/;
      return 1 if (-e $tf);
      $tf =~ s/\.tcc\.in$/\.mm.in/;
      return 1 if (-e $tf);

      #causes insanity on case-insensitive filesystems
      #$tf =~ s/\.mm\.in$/\.C.in/;
      #return 1 if (-e $tf);
    }
    if ($f =~ m/\.h\.cmake$/) {
      my ($tf) = $f;
      $tf =~ s/\.h\.cmake$/\.cpp\.cmake/;
      return 1 if (-e $tf);
      $tf =~ s/\.cpp\.cmake$/\.cc.cmake/;
      return 1 if (-e $tf);
      $tf =~ s/\.cc\.cmake$/\.cxx.cmake/;
      return 1 if (-e $tf);
      $tf =~ s/\.cxx\.cmake$/\.tcc.cmake/;
      return 1 if (-e $tf);
      $tf =~ s/\.tcc\.cmake$/\.mm.cmake/;
      return 1 if (-e $tf);

      #causes insanity on case-insensitive filesystems
      #$tf =~ s/\.mm\.cmake$/\.C.cmake/;
      #return 1 if (-e $tf);
    }
  }

  return 1 if ($t eq "c") && (($f =~ m/\.c$/ || $f =~ m/\.h$/));

  #easy checks for the rest of them
  return 1 if (($t eq "desktop")   && ($f =~ m/\.desktop$/));
  return 1 if (($t eq "designer")  && ($f =~ m/\.ui$/));
  return 1 if (($t eq "kconfigxt") && ($f =~ m/\.kcfg$/));
  return 1 if (($t eq "po")        && ($f =~ m/\.po$/));
  return 1 if (($t eq "kpartgui")  && ($f =~ m/\.rc$/));
  return 1 if (($t eq "qml")       && ($f =~ m/\.qml$/));
  return 1 if (($t eq "qdoc")      && ($f =~ m/\.qdoc$/));
  return 1
    if (($t eq "cmake")
    && ($f =~ m/CMakeLists\.txt$/ || $f =~ m/\.cmake$/ || $f =~ m/\.cmake\.in$/));
  return 1 if (($t eq "perl")
    && ($f =~ m/\.pl$/i || $f =~ m/\.pm$/i));
  return 1 if (($t eq "python") && ($f =~ m/\.py$/));
  return 1 if (($t eq "json")   && ($f =~ m/\.json$/i));
  return 1 if (($t eq "svg")    && ($f =~ m/\.svg$/));

  &userMessage("BAD FILETYPE PASSED TO fileTypeIs()") if (&fileType($f) eq "");
  return 0;
}

#is the file a C source (explicitly NOT C++ and NOT a C/C++ include)?
sub isCSource
{
  my ($f) = @_;

  return 1 if ($f =~ m/\.c$/);
  return 0;
}

#is the file a C/C++ include?
sub isCInclude
{
  my ($f) = @_;

  return 1 if ($f =~ m/\.(?:h|hxx|hpp|H\+\+)$/);
  return 1 if ($f =~ m/\.(?:h|hxx|hpp|H\+\+)\.in$/);
  return 1 if ($f =~ m/\.(?:h|hxx|hpp|H\+\+)\.cmake$/);

  return 0;
}

# return a string containing all the supported files found in specified dirs
# the files are newline-separated.
sub findFiles
{
  my (@dirs) = @_;
  $dirs[0] =~ s:\\\+:+:g;    #unescape '+'
  push(@dirs, getcwd) if ($#dirs < 0);

  @tmp = ();
  find(\&aok, @dirs);

  ## no critic
  sub aok
  {
    -f && !-d && push(@tmp, $File::Find::name);
  }
  ## use critic
  my ($l) = "";
  foreach my ($i) (@tmp) {
    $l = "$l" . "$i\n" if (&fileType($i) && ($i !~ m+\.(git|svn|hg)/+));
  }
  return $l;
}

# find file by regex in the specified dir. return 1 if any hits found; 0 otherwise
sub findFileByRegex
{
  my ($regex, @dirs) = @_;
  my ($top) = &topOfProject($dirs[0]);
  if (!$top) {
    $top = &guessTopOfProject($dirs[0]);
    if (!$top) {
      return 0;
    }
  }

  @tmp = ();
  find(\&aok2, ($top));

  ## no critic
  sub aok2
  {
    -f && !-d && push(@tmp, $File::Find::name);
  }
  ## use critic
  my ($l) = "";
  foreach my ($i) (@tmp) {
    return 1 if ($i =~ m/$regex/);
  }
  return 0;
}

# asOf function: return nicely formatted string containing the current time
sub asOf
{
  my $locale = setlocale(LC_TIME);
  setlocale(LC_TIME, "C");
  my $time = strftime("%B %d %Y %H:%M:%S %Z", localtime(time()));
  setlocale(LC_TIME, $locale);
  return $time;
}

my ($krazy)     = '';
my ($help)      = '';
my ($version)   = '';
my ($priority)  = 'all';
my ($strict)    = 'all';
my ($checksets) = '';
my ($explain)   = '';
my ($installed) = '';
my ($quiet)     = '';
my ($verbose)   = '';

sub parseArgs
{

  exit 1
    if (
    !GetOptions(
      'krazy'        => \$krazy,
      'help'         => \$help,
      'version'      => \$version,
      'priority=s'   => \$priority,
      'strict=s'     => \$strict,
      'check-sets=s' => \$checksets,
      'explain'      => \$explain,
      'installed'    => \$installed,
      'verbose'      => \$verbose,
      'quiet'        => \$quiet
    )
    );

  if (!$help && !$version && !$explain) {
    if (!$krazy) {
      print "Checker not called as part of Krazy... exiting\n";
      exit(1);
    }
  }

  if (!&validatePriorityType($priority)) {
    my ($lst) = &priorityTypeStr();
    print "Bad priority level \"$priority\" specified... exiting\nChoices for priority are: $lst\n";
    exit(1);
  }

  if (!&validateStrictType($strict)) {
    my ($lst) = &strictTypeStr();
    print "Bad strictness level \"$strict\" specified... exiting\nChoices for strict are: $lst\n";
    exit(1);
  }

  my ($badsets) = validateCheckSets($checksets);
  if ($badsets) {
    my ($lst) = &checksetTypeStr();
    print "Bad checkset(s) \"$badsets\" specified... exiting\nChoices for checksets are: $lst\n";
    exit(1);
  }

}

sub helpArg      {return $help;}
sub versionArg   {return $version;}
sub priorityArg  {return $priority;}
sub strictArg    {return $strict;}
sub explainArg   {return $explain;}
sub installedArg {return $installed;}
sub quietArg     {return $quiet;}
sub verboseArg   {return $verbose;}

sub exportTypeStr
{
  return join ', ', @Exports;
}

sub validateExportType
{
  my ($export) = @_;
  if ($export) {
    $export = lc($export);
    return grep {$_ eq $export} @Exports;
  }
  return 0;
}

sub priorityTypeStr
{
  return join ', ', @Priorities;
}

sub validatePriorityType
{
  my ($priority) = @_;
  if ($priority) {
    $priority = lc($priority);
    return grep {$_ eq $priority} @Priorities;
  }
  return 0;
}

sub strictTypeStr
{
  return join ', ', @Stricts;
}

sub validateStrictType
{
  my ($strict) = @_;
  if ($strict) {
    $strict = lc($strict);
    return grep {$_ eq $strict} @Stricts;
  }
  return 0;
}

sub outputTypeStr
{
  return join ', ', @Outputs;
}

sub validateOutputType
{    #output type doesn't consider verbosity (except in the 'quiet' case)
  my ($output) = @_;
  if ($output) {
    $output = lc($output);
    return grep {$_ eq $output} @Outputs;
  }
  return 0;
}

sub cppIncludeOrderTypeStr
{
  return join ', ', @CppIncludeOrderTypes;
}

sub validateCppIncludeOrderType
{
  my ($order) = @_;
  if ($order) {
    $order = lc($order);
    return grep {$_ eq $order} @CppIncludeOrderTypes;
  }
  return 0;
}

# print a pretty list of supported check-sets with descriptions
sub prettyPrintCheckSetsList()
{
  foreach my ($set) (&checkSetsList()) {
    printf("%12.12s: %s\n", $set, &checkSetDesc($set));
  }
}

# return the description string for the specified check-set
sub checkSetDesc()
{
  my ($s) = @_;

  if ($s eq "c++") {
    return "Pure C/C++ source";
  } elsif ($s eq "qt") {
    return "Qt source (C++, QML, Qt Designer, Qt Doc)";
  } elsif ($s eq "kde") {
    return "KDE/Qt source (C++, Qt Designer, FDO desktop files, etc.)";
  } elsif ($s eq "kde-ci") {
    return "KDE source - CI specific";
  } elsif ($s eq "foss") {
    return "Free and open source software (FOSS)";
  }
  return "";
}

sub checksetTypeStr
{
  return join ', ', @Sets;
}

sub checkSetsList()
{
  return @Sets;
}

sub validateCheckSet
{
  my ($set) = @_;
  if ($set) {
    $set = lc($set);
    return grep {$_ eq $set} @Sets;
  }
  return 0;
}

sub dedeprecateCheckSet
{
  my ($set) = @_;
  $set = "qt"  if ($set =~ m/^qt[[:digit:]]/);
  $set = "kde" if ($set =~ m/^kde[[:digit:]]/);
  return $set;
}

sub dedeprecateCheckSets
{
  my ($checksets) = @_;
  my (@newsets)   = ();
  foreach my ($set) (split(",", $checksets)) {
    push(@newsets, &dedeprecateCheckSet($set));
  }
  return @newsets;
}

sub dedeprecateCheckSetsStr
{
  my (@checksets) = @_;
  return join ', ', &dedeprecateCheckSets(@checksets);
}

sub validateCheckSets
{
  my ($checksets) = @_;
  my (@badsets)   = ();
  foreach my ($set) (split(",", $checksets)) {
    my ($newset) = &dedeprecateCheckSet($set);
    print STDERR "Deprecated check-set \"$set\". Please use \"$newset\" instead\n" if ($set ne $newset);
    if (!&validateCheckSet($newset)) {
      push(@badsets, $set);
    }
  }
  return join ', ', @badsets;
}

sub usingCheckSet
{
  my (@sets) = @_;
  foreach my ($s) (@sets) {
    foreach my ($set) (split(",", $checksets)) {
      if ($s eq $set) {
        return 1;
      }
    }
  }
  return 0;
}

sub usingQtCheckSet
{
  foreach my ($set) (split(",", $checksets)) {
    if ($set =~ m/^qt/i) {
      return 1;
    }
  }
  return 0;
}

sub usingKDECheckSet
{
  foreach my ($set) (split(",", $checksets)) {
    if ($set =~ m/^kde/i) {
      return 1;
    }
  }
  return 0;
}

#search for any one of the strings in specified file. case matters.
#returns:
# the line number (>0) of the first match
# 0 if there is no match
# -1 if the file cannot be opened for reading
sub linesSearchInFile
{
  my ($f, @lines) = @_;

  open my $fh, '<:encoding(UTF-8)', $f or return -1;
  my ($cnt) = 0;
  while (<$fh>) {
    $cnt++;
    chomp($_);
    for my ($l) (@lines) {
      if ($_ =~ m/$l/) {
        close($fh);
        return $cnt;
      }
    }
  }
  close($fh);
  return 0;
}

#search for any one of the strings in specified CMakeLists.txt file.
# case is ignored. lines with leading whitespace are merged onto previous line
#returns:
# the line number (>0) of the first match
# 0 if there is no match
# -1 if the file cannot be opened for reading
sub linesCaseSearchInFile
{
  my ($f, @lines) = @_;

  open my $fh, '<:encoding(UTF-8)', $f or return -1;
  my ($cnt) = 0;
  while (<$fh>) {
    $cnt++;
    chomp($_);
    for my ($l) (@lines) {
      if ($_ =~ m/$l/i) {
        close($fh);
        return $cnt;
      }
    }
  }
  close($fh);
  return 0;
}

#search for all the strings in specifie file. case is ignored.
#returns:
# 1 if all the strings are found in the file
# 0 if at least 1 string is not found in the file
# -1 if the file cannot be opened for reading
sub allLinesCaseSearchInFile
{
  my ($f, @lines) = @_;

  open my $fh, '<:encoding(UTF-8)', $f or return -1;
  my (@data_lines) = <$fh>;
  close($fh);

  my ($cnt)      = 0;
  my ($outLines) = "";
  foreach my ($line) (@data_lines) {
    chomp($line);

    # squeeze the spaces from start of line
    if (!($line =~ s/^\s+/ /)) {

      # add newline (not for first line)
      $outLines = $outLines . "\n" if $. != 1;
    }
    $outLines = $outLines . $line;
  }
  close($fh);

  for my $sqLine (split('\n', $outLines)) {
    chomp($sqLine);
    my ($offset) = -1;
    for my ($l) (@lines) {
      $offset++;
      if ($sqLine =~ m/$l/i) {
        splice(@lines, $offset, 1);
      }
    }
    if ($#lines < 0) {
      last;
    }
  }

  if ($#lines == -1) {
    return 1;
  } else {
    return 0;
  }
}

#make a best guess at the checkset to use for the specified project
sub guessCheckSet
{
  my ($in) = @_;    # dir to process

  #default checkset
  my ($checkset) = '';

  my ($project) = &basename($in);
  my ($modpath) = &dirname($in);
  my ($module)  = &basename($modpath);

  #look up 1 level if a bundled-app project
  $in = $modpath if ($module eq "calligra" || $module eq "krita" || $module eq "kexi");

  my ($cmakepath) = catfile($in, "CMakeLists.txt");
  my ($qmakepath) = catfile($in, $project . ".pro");
  my ($autopath)  = catfile($in, "autogen.py");
  my (@fosspaths) = (
    catfile($in, "COPYING"),
    catfile($in, "COPYING.GPL"),
    catfile($in, "COPYING.LGPL"),
    catfile($in, "COPYING.LIB"),
    catfile($in, "LICENSE.GPL.txt"),
    catfile($in, "LICENSE.LGPL.txt"),
  );

  #CMake buildsystems
  if (-e $cmakepath) {
    if (&allLinesCaseSearchInFile($cmakepath, ("find_package\\s*\\(\\s*KF")) > 0) {
      $checkset = "kde";
    } elsif (&allLinesCaseSearchInFile($cmakepath, ("find_package\\s*\\(\\s*Qt")) > 0) {
      $checkset = "qt";
    } elsif (&allLinesCaseSearchInFile($cmakepath, ("project\\s*\\(.*CXX")) > 0) {
      $checkset = "c++";
    } elsif (&allLinesCaseSearchInFile($cmakepath, ("enable_language\\s*\\((\\s*CXX")) > 0) {
      $checkset = "c++";
    }
  } elsif (-e $qmakepath) {
    $checkset = "qt";
  } elsif (bsd_glob(catfile($in, "*.pro"))) {
    $checkset = "qt";
  } elsif (-e $autopath) {
    $checkset = "qt";
  }

  my ($fcheckset) = '';
  for my ($p) (@fosspaths) {
    if (-e $p) {
      $fcheckset = "foss";
      last;
    }
  }

  if ($checkset ne "") {
    $checkset = join(',', $checkset, $fcheckset) if ($fcheckset ne "");
  } elsif ($fcheckset ne "") {
    $checkset = $fcheckset;
  }
  return $checkset;
}

#print the Issue, text export
sub printIssueTextEdit()
{
  my ($fullpath, $line, $desc, $checker, $hint, $offendingcode) = @_;
  print $fullpath . ":" . $line . ":" . $desc . " [" . $checker . "]\n";
}

#print the Issue, figures out the export mode and does the right thing
sub printIssue()
{
  my ($export, $fullpath, $line, $desc, $checker, $hint, $offendingcode) = @_;
  if ($export eq "text") {
    &printIssueText($fullpath, $line, $desc, $checker, $hint, $offendingcode);
  } elsif ($export eq "textlist") {
    &printIssueTextList($fullpath, $line, $desc, $checker, $hint, $offendingcode);
  } elsif ($export eq "textedit") {
    &printIssueTextEdit($fullpath, $line, $desc, $checker, $hint, $offendingcode);
  } else {
    &userMessage("BAD EXPORT PASSED TO printIssue()");
  }
}

1;
