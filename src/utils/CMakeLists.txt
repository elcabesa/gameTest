target_sources(sfml PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/occupancy.cpp
)

target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})