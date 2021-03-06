cmake_minimum_required (VERSION 2.6)
project (SoundDemo)

add_subdirectory(../../../../oxygine-framework oxygine-framework)

add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_subdirectory(../../../ oxygine-sound)
include_directories(${OXYGINESOUND_INCLUDE_DIRS})



SET(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(SoundDemo
	../src/entry_point.cpp
	../src/example.h 
	../src/example.cpp
	../src/test.cpp
	../src/test.h
)


set(CMAKE_CXX_FLAGS "-Wno-warn-absolute-paths")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
	SET(linkFlags "-g4 ")
else() # Either MinSizeRel, RelWithDebInfo or Release, all which run with optimizations enabled.
	SET(linkFlags "-O2 ")
endif()

set(linkFlags "${linkFlags} -s NO_EXIT_RUNTIME=1 -s FULL_ES2=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -Wno-warn-absolute-paths -s TOTAL_MEMORY=64435456")

set_target_properties(SoundDemo PROPERTIES LINK_FLAGS "${linkFlags}")



target_link_libraries(SoundDemo ${OXYGINE_CORE_LIBS} ${OXYGINESOUND_LIBS})
em_link_pre_js(SoundDemo ${CMAKE_CURRENT_SOURCE_DIR}/data.js
	${CMAKE_CURRENT_SOURCE_DIR}/sound.js
	${CMAKE_CURRENT_SOURCE_DIR}/js2cpp.js
	${CMAKE_CURRENT_SOURCE_DIR}/preloading.js)