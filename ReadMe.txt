Krazy - Sanity check your source code
=====================================

Introduction
------------
Krazy scans source code looking for issues that should be fixed
for reasons of policy, good coding practice, optimization, or any other
good reason.  In typical use, Krazy simply counts up the issues
and provides the line numbers where those issues occurred in each
file processed.  With the verbose option, the offending content will
be printed as well.

Krazy uses "sanity checker programs" which are small plugin programs
to do the real work of the scanning.  It is easy to write your own plugins
and tell Krazy how to use them.

Installation
------------
See the included Install.txt file.

Documentation
-------------
The documentation is included with the installed man pages.
See krazy2(1) and krazyrc(3)

Resources
---------
Krazy is hosted by GitHub at https://github.com/Krazy-collection/krazy,
there you will find a wiki https://github.com/Krazy-collection/krazy/wiki
and an issue tracker https://github.com/Krazy-collection/krazy/issues.

Visit the Krazy home page at http://krazy-collection.github.io/krazy

Donate Plugins!  We'll include your cool plugin with this package if it
has an OSS-approved license!

License
-------
Copyright (C) 2005-2016 by Allen Winter <winter@kde.org>

See the included License.txt file.
