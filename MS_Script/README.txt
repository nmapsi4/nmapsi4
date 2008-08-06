### Requirements

* Download and install "NSIS (Nullsoft Scriptable Install System)" from http://nsis.sourceforge.net/Download

* Download and install "HM NIS Edit" from http://hmne.sourceforge.net/index.php#Download

* Download "Nmap command-line zipfile" from http://nmap.org/download.html

### Preliminary steps
    
In order to compile NmapSI4 use one of this howto:
"http://code.google.com/p/nmapsi4/wiki/BuildNmapSI4usingCmakeonWindows"
"http://code.google.com/p/nmapsi4/wiki/BuildNmapsi4inWindows"

Extract nmap-******.zip, (eg in "C:\nmap-zip")

Create a folder for files containing nmapsi4 previously compiled (eg "C:\nmap-zip\nmapsi4-0.1-win32" o "C:\nmap-zip\nmapsi4-svn")

Create a folder where you'll copy within scripts from "MS_Script/" folder, (eg "C:\nmap-zip\nsis")

Create a folder "pack", (eg "C:\nmap-zip\pack"), here you will find the setup files

### Let's go

In the folder "MS_script/" you will find some files:
    * AddToPath.nsh, final.ini, shortcuts.ini: these scripts are provided by the Nmap team and are used for the creation of the final installer;
    * Nmap-nmapsi4_0.1.nsi, Nmap-nmapsi4_svn.nsi: scripts to create the complete installer, respectively for version 0.1 (branches) or svn (trunk)
    * nmapsi-0.1-win32.nsi, nmapsi4-svn-win32.nsi: scripts to create the nmapsi4 installer, respectively for version 0.1 (branches) or svn (trunk), this installer will be used by the final installer.
    
Open the file Nmap-nmapsi4_0.1 with "HM NIS Edit" and compile the script (Ctrl + F9), if you used the same paths you should not have errors and you will find in the "pack/" the executable "Nmap_with_NmapSI4_GUI. exe "ready to be used.

P.S. In the file "nmapsi4-svn-win32.nsi" there is the label [ ! define PRODUCT_VERSION "svnDATE-REV_NUM" ], where DATE is the date in the form DMY and REV_NUM the number of revision.

### Good Luck!

########################################################################
# '|Br4nD|':                                                 #
# 02:16                                                                #
# Inno remember to write a README with MS windows script istructions   #
# 02:16                                                                #
# inside the script directory                                          #
# 02:16                                                                #
# a small README                                                       #
# Eldiablo:                                                            #
# 02:16                                                                #
# uff I don't know the real script istructions  :P                     #
# '|Br4nD|':                                                 #
# 02:16                                                                #
# ahahahahahahaha :D                                                   #
########################################################################
