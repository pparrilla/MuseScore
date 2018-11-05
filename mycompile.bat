PATH=C:\Qt\Qt5.11.2\5.11.2\mingw53_32\bin;C:\Qt\Qt5.11.2\Tools\mingw530_32\bin;C:\MinGW\bin;C:\Program Files\CMake\bin;%SystemRoot%\system32;%SystemRoot%;%SystemRoot%\System32\Wbem;%SYSTEMROOT%\System32\WindowsPowerShell\v1.0\;C:\msys32\usr\bin;C:\Program Files (x86)\WiX Toolset v3.11\bin;C:\Program Files\Git\cmd

#mingw32-make -f Makefile.mingw revision
mingw32-make -f Makefile.mingw release BUILD_PCH=ON
mingw32-make -f Makefile.mingw installrelease
mingw32-make -f Makefile.mingw package
