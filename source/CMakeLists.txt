SET(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

project( identifier )

# set custom opencv directory
set(OpenCV_DIR "/home/hitech/files/opencv_3_3_1_build") 
set(SOURCES_FILE "src/object.cpp")

find_package(OpenCV REQUIRED PATHS /usr/share/OpenCV NO_DEFAULT_PATH) 

# add_executable( identifier main.cpp src/object.cpp)
add_executable( identifier main.cpp ${SOURCES_FILE})


target_link_libraries( identifier ${OpenCV_LIBS} )

 # to include a directory to the target
target_include_directories(identifier PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )



 