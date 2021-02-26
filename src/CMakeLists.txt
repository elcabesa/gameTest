find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)

add_executable(sfml 
    application.cpp
    gui.cpp
    main.cpp
    player.cpp
    world.cpp
)
target_link_libraries (sfml sfml-graphics sfml-window sfml-system tgui spdlog::spdlog_header_only)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TGUI/themes/Black.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/gui)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TGUI/themes/Black.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/gui)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/man.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

add_subdirectory(systems)
add_subdirectory(utils)
add_subdirectory(spdlog)
add_subdirectory(TGUI)

# Make sure the compiler can find include files for our sfml library
# when other libraries or executables link to sfml
target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
