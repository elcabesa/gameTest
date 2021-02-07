target_sources(sfml PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/health.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/movement.cpp
)

target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})