#!/bin/bash
# Francesco Cecconi <francesco.cecconi@gmail.com>
# copy required Qt4 into Mac OS bundle package

BUNDLE="nmapsi4.app/Contents/";
LIB_DEFAULT="/Library/Frameworks/";
VERSION_STRING="/Versions/4/";

DEP[1]="QtWebKit.framework";
DEP[2]="QtGui.framework";
DEP[3]="QtXmlPatterns.framework";
DEP[4]="QtNetwork.framework";
DEP[5]="QtCore.framework";
#DEP[6]="QtDeclarative.framework";
#DEP[7]="QtScript.framework";
#DEP[8]="QtSvg.framework";
#DEP[9]="QtXml.framework";
#DEP[10]="QtSql.framework";


echo -e "\033[00;31mSTATUS:: copy dependencies\033[00m"
mkdir -p $BUNDLE"/Frameworks/"
for i in "${DEP[@]}"; do 
	SOURCE="$LIB_DEFAULT$i/";
	DEST=$BUNDLE"Frameworks/"$i;
	cp -R $SOURCE $DEST;
done

echo -e "\033[00;31mSTATUS:: change dependencies path\033[00m"
BINARY_NAME=$BUNDLE"MacOS/nmapsi4";
for i in "${DEP[@]}"; do 
	SOURCE=$i$VERSION_STRING${i%.*};
	DEST="@executable_path/../"$i$VERSION_STRING${i%.*};
	install_name_tool -change $SOURCE $DEST $BINARY_NAME
done
