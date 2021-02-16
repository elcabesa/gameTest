find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)

add_executable(sfml main.cpp application.cpp )
target_link_libraries (sfml sfml-graphics sfml-window sfml-system tgui)

add_subdirectory(systems)
add_subdirectory(utils)
add_subdirectory(TGUI)

# Make sure the compiler can find include files for our sfml library
# when other libraries or executables link to sfml
target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
