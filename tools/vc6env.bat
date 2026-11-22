@echo off
rem Sets up the portable MSVC 6.0 SP5 toolchain (cl 12.00.8804, c1/c1xx/c2 build 8966) from tools\vc6.
rem Usage: call tools\vc6env.bat
set VC6=%~dp0vc6
set PATH=%VC6%\VC98\BIN;%VC6%\COMMON\MSDEV98\BIN;%PATH%
set INCLUDE=%VC6%\VC98\INCLUDE;%VC6%\VC98\MFC\INCLUDE;%VC6%\VC98\ATL\INCLUDE
set LIB=%VC6%\VC98\LIB;%VC6%\VC98\MFC\LIB
