cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0054 NEW)
project(netLink)

# set(OUTPUT_DIR out)
set(SOURCE_DIR src)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

include_directories(PUBLIC include)
aux_source_directory(${SOURCE_DIR} SOURCES)

add_library(netLink ${SOURCES})

# add_library(shared SHARED ${SOURCES})
# add_library(static STATIC ${SOURCES})
# set_target_properties(shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
# set_target_properties(static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
# set_target_properties(shared PROPERTIES DEBUG_POSTFIX -d)
# set_target_properties(static PROPERTIES DEBUG_POSTFIX -d)

if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-O3 -std=c++11)
endif()

#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#    add_definitions(-stdlib=libc++)
#endif()

# file(GLOB INCLUDE_HEADERS "include/*.h")
# install(FILES ${INCLUDE_HEADERS} DESTINATION include/${CMAKE_PROJECT_NAME})
# install(TARGETS shared DESTINATION lib)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
#target_include_directories(netLink PUBLIC
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#	$<INSTALL_INTERFACE:include>
#	PRIVATE include)

target_link_libraries(netLink)
