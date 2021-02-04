export CC=clang-10
export CXX=clang++-10

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

