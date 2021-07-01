#!/bin/bash
fullpath=".\bin\$1.xex"
./build.sh $1

cd bin
wine c:/Altirra/Altirra $1.ATR
