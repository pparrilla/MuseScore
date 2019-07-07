cls
echo %time%

PATH=C:\Qt\5.12.3\mingw73_32\bin;C:\Qt\Tools\mingw730_32\bin
PATH=%PATH%;C:\Program Files\CMake\bin;C:\Program Files\Git\cmd
PATH=%PATH%;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\;C:\msys32\usr\bin
PATH=%PATH%;C:\Program Files (x86)\WiX Toolset v3.11\bin

mingw32-make -f Makefile.mingw revision
mingw32-make -f Makefile.mingw release BUILD_PCH=ON
mingw32-make -f Makefile.mingw installrelease
mingw32-make -f Makefile.mingw package
