export CC=clang-10
export CXX=clang++-10

mkdir build-dbg
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
