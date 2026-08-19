# Krazy Installation Instructions

Make sure you have the required software installed first.
See the "Requirements" section below.

## "Easy" Krazy Installation Instructions

The easiest and safest way to install Krazy is by using the simple
install script called 'install.sh'.

``` shell
  % ./install.sh
```

which will install by default into /usr/local/Krazy2, making sure
your user had permissions to write into /usr/local.

Or, set another installation directory by passing it to 'install.sh' on
the command line, like so:

```shell
  % ./install.sh /another/location
```

Next you must add /usr/local/Krazy2/bin (or /another/location/bin)
to your execute path.

```shell
  % export PATH=$PATH:/usr/local/Krazy2/bin
    or
  % export PATH=$PATH:/another/location/bin
```

Now, the `krazy2 --list` command should show you a list of available plugins.

## "Hard" Krazy Installation Instructions

Read the `install.sh` script included with this package
and try to figure it out yourself. :)

## Requirements

As of this writing there are several Perl modules required that may
not normally ship with a typical O/S distribution. These modules are
checked for when the top-level Makefile.PL is processed.
The modules are:

* HTML::Parser
* File::Find
* File::Spec
* Tie::IxHash
* XML::LibXML
* YAML
* JSON
* Digest::MD5
* (possibly more.  see the top-level Makefile.PL)

Also, the programs desktop-file-validate and xmllint are required.
Please install them and make sure they are in your execute PATH.

* desktop-file-validate: install the desktop-file-utils package or build it from
source. See <https://www.freedesktop.org/software/desktop-file-utils/releases>.

* xmllint: install the libxml2 package or build it from source.
See <https://gitlab.gnome.org/GNOME/libxml2/-/wikis/home>.

Otherwise, Krazy should work with all known UNIXish operating systems
where Perl s available.
