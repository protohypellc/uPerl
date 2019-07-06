@echo off

Set SixtyFourBit=%1%
Set Makefile=%2%
Set Action=%3%

Set MakefileDrive=%Makefile:~1,2%
Set MakefileDir=%Makefile:~3,-9%
Set VCVars=vcvars32

if %SixtyFourBit%=="1" Set VCVars=vcvars64
if %SixtyFourBit%=="0" Set VCVars=vcvars32

call %VCVars%

if %SixtyFourBit%=="1" Set VC_WIN64=define
if %SixtyFourBit%=="1" Set VC_64_BIT_INT=define

if %SixtyFourBit%=="0" Set VC_WIN64=undef
if %SixtyFourBit%=="0" Set VC_64_BIT_INT=undef

%MakefileDrive%

cd "%MakefileDir%"

if %Action%=="build" call nmake 
if %Action%=="clean" call nmake distclean
if %Action%=="rebuild" call nmake rebuild

if %Action%=="clean" GOTO EXIT
if %SixtyFourBit%=="1" GOTO WIN64
if %SixtyFourBit%=="0" GOTO WIN32

:WIN32
copy ..\lib\Core\Perl522s.lib ..\..\Release\Perl522s.lib
copy ..\perl522.dll ..\..\Release\perl522.dll
copy ..\perl-static.lib ..\..\Release\perl-static.lib
copy ..\pp_sys.obj ..\..\Release\pp_sys.obj
copy ..\util.obj ..\..\Release\util.obj
copy .\win32.obj ..\..\Release\win32.obj
copy .\win32sck.obj ..\..\Release\win32sck.obj
copy .\perllib.obj ..\..\Release\perllib.obj
goto:eof

:WIN64
copy ..\lib\Core\Perl522s.lib ..\..\x64\Release\Perl522s.lib
copy ..\perl522.dll ..\..\x64\Release\perl522.dll
copy ..\perl-static.lib ..\..\x64\Release\perl-static.lib
copy ..\pp_sys.obj ..\..\x64\Release\pp_sys.obj
copy ..\util.obj ..\..\x64\Release\util.obj
copy .\win32.obj ..\..\x64\Release\win32.obj
copy .\win32sck.obj ..\..\x64\Release\win32sck.obj
copy .\perllib.obj ..\..\x64\Release\perllib.obj
goto:eof

:EXIT


