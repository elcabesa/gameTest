target_sources(sfml PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/collisionDetector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/health.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/movement.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
)

target_include_directories (sfml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})