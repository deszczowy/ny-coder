SET qtpathDir=[[Qt binaries e.g. ..\Qt\5.11.0\mingw53_32\bin ]]
SET nycdebugDir=[[Project debug output e.g. ..\build-nyq-Desktop_Qt_5_11_0_MinGW_32bit-Debug\debug ]]
SET nycsourceDir=[[Project source, root directory with 'nyq.pro' file ]]
SET releaseDir=[[Where to gather binaries ]]

mkdir %releaseDir%

copy "%qtpath%\libgcc_s_dw2-1.dll" "%releaseDir%\*.*"
copy "%qtpath%\libstdc++-6.dll" "%releaseDir%\*.*"
copy "%qtpath%\libwinpthread-1.dll" "%releaseDir%\*.*"
copy "%qtpath%\Qt5Cored.dll" "%releaseDir%\*.*"
copy "%qtpath%\Qt5Guid.dll" "%releaseDir%\*.*"
copy "%qtpath%\Qt5Sqld.dll" "%releaseDir%\*.*"
copy "%qtpath%\Qt5Widgetsd.dll" "%releaseDir%\*.*"
copy "%nycsourceDir%\nyc.setup" "%releaseDir%\*.*"
copy "%nycsourceDir%\res\themes\carbon.theme" "%releaseDir%\*.*"
copy "nyc.exe" "%releaseDir%\*.*"