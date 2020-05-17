
# HACK for netLink.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../External/netLink/include)
# HACK for jsonxx.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/jsonxx)

add_library(JSONAPI_lib
        JSONAPI.cpp
        )