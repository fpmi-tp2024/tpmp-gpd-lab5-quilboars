cd ../
rm -drf build_linux
mkdir build_linux
cd build_linux
cmake ../
cmake --build ./ --config Release