#!/bin/bash

tsDir="src/ts"
templateDir="$tsDir/template/nmapsi4.ts"
translationsPath="$templateDir "

for tsfile in $(ls $tsDir); do
        if [ $tsfile != "po" ] && [ $tsfile != "template" ]; then
            translationsPath="$translationsPath$tsDir/$tsfile "
        fi
done

lupdate -recursive -no-obsolete src/ -ts $translationsPath
cd tools/
ruby convert_translations.rb ts2po