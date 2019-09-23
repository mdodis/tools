@echo off

set opts=-FC -GR- -EHa- -nologo -Zi
set code=%cd%
pushd .
cl %opts% %code%\main.c -Fetaah
popd
