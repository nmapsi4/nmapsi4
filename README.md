# NmapSi4

## Description:

NmapSi4 is a complete Qt-based Gui with the design goals to provide a complete nmap 
interface for Users, in order to management all options of this powerful 
security net scanner.

## Download:

Github: <https://github.com/nmapsi4/nmapsi4/releases>

SourceForge: <https://sourceforge.net/projects/nmapsi/>

## HomePage:

<https://github.com/nmapsi4/nmapsi4>

## Required for build:

* cmake >= 2.8.2
* Qt5 >= 5.7
* webenginewidgets

## Required for runtime:

* nmap + nping >= 6.00
* dig package (debian: dnsutils)

## Build on GNU/Linux:

    $ cd nmapsi4
    $ cd tools/
    $ ./cmake_verbose_script.sh

### OR

    $ cd nmapsi4
    $ mkdir build
    $ cd build/

### AND 

    $ make
    $ make install (only root) or create a package for your disto ;)


## Info for admin desktop file:

nmapsi4-admin.desktop uses by default kdesu for execute nmap with root uid. 

However, it is possible change kdesu in the desktop file:
nmapsi4-admin.desktop: Exec=kdesu QTWEBENGINE_DISABLE_SANDBOX=1 nmapsi4 with Exec=[what you want] nmapsi4

## Contributing

Report bugs on <https://github.com/nmapsi4/nmapsi4/issues>

Clone git repository with:

    git clone git://github.com/nmapsi4/nmapsi4.git

## Author

- Francesco Cecconi <francesco.cecconi@gmail.com>
