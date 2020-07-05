cmake_minimum_required(VERSION 3.0)

project(rest-api)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "BOOST NOT FOUND.")
endif()

#message(STATUS ${Boost_INCLUDE_DIRS})
#message(STATUS ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
include_directories("/usr/local/src/vcpkg/installed/x64-linux/include")
include_directories(".")

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} 
    Boost::program_options
    Boost::filesystem)
