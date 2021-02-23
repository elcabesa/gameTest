target_sources(sfml PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/quadtree.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/resourceManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/statistics.cpp
)

target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})