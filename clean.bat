REM clean up after Visual Studio
erase *.sdf
erase *.opensdf

rd .\ipch /s /q

REM remove the build folders (has compiled .exe files in it)
rd .\Debug /s /q
rd .\Release /s /q

rd .\NibbleAndAHalf\Debug /s /q
rd .\NibbleAndAHalf\Release /s /q

REM Remove the x64 build folder
rd .\x64 /s /q
rd .\NibbleAndAHalf\x64 /s /q

