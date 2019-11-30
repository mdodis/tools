@echo off

set opts=-FC -GR- -EHa- -nologo -Zi
set code=%cd%
pushd output
cl %opts% %code%\target_.cc -Febuild.output
popd
