mkdir build
cd build

cmake ..
cmake --build .

cd ..
rmdir bin\Debug\resources /s /q
xcopy "source\resources\" "bin\Debug\resources\" /Y /e

pause