
add_executable(sfml main.cpp )
target_link_libraries (sfml sfml-graphics sfml-window sfml-system)

add_subdirectory(systems)

# Make sure the compiler can find include files for our sfml library
# when other libraries or executables link to sfml
target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
