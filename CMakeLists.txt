cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(my_design)

set (CMAKE_BUILD_TYPE Debug)

set (CMAKE_CXX_STANDARD 14)

include_directories(./)
set (INC inc/tree-3.7/ inc/logger inc/search)

add_definitions(-DCPPLOG_FILTER_LEVEL=LL_DEBUG)   

#search
set(SOURCES app/test_search.cpp)
add_executable(test_tree_search ${SOURCES})
include_directories(tree_search PRIVATE ${INC})

#insert
set(SOURCES_INSERT app/test_insert.cpp)
add_executable(test_tree_insert ${SOURCES_INSERT})
include_directories(test_tree_insert PRIVATE ${INC})

#substitute
set(SOURCES_SUBSTITUTE app/test_search_substitute.cpp)
add_executable(test_tree_search_substitute ${SOURCES_SUBSTITUTE})
include_directories(test_tree_search_substitute PRIVATE ${INC})


#search substitute, add , erase
set(SOURCES_ADD_ERASE app/test_search_add_erase.cpp)
add_executable(test_tree_search_add_erase ${SOURCES_ADD_ERASE})
include_directories(test_tree_search_add_erase PRIVATE ${INC})


#performance search, insert, substitute, add , erase
set(SOURCES_ADD_ERASE app/test_performance.cpp)
add_executable(test_performance ${SOURCES_ADD_ERASE})
include_directories(test_performance PRIVATE ${INC})
