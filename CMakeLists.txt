#cmake configuration

cmake_minimum_required (VERSION 3.10)

#project configuration
project (sfml)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -m64" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pedantic -Wall -Wextra -g" )

add_subdirectory(src)

 
