#!/bin/bash

MSYS_LIB_DIR="/mingw64/bin"

for x in `ls *.dll` ; do
	echo "checking: $x"
	md5sum $x
	tmp1=$(md5sum "$x" | awk '{print $1}' )
	md5sum $MSYS_LIB_DIR/$x
	tmp2=$(md5sum "$MSYS_LIB_DIR/$x" |awk '{print $1}' )
	if [[ "$tmp1" == "$tmp2" ]]; then
		echo "FILE MATCH - $x"
	else
		cp -pv $MSYS_LIB_DIR/$x .
	fi
	echo "------------"
done
