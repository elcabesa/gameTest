target_sources(sfml PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/occupancy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/quadtree.cpp
)

target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})