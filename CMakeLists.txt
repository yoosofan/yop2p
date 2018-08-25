cmake_minimum_required(VERSION 3.7)
project(yop2p VERSION 0.0.1 LANGUAGES CXX)
#add_library(yop2p src/test3.cpp)
add_executable(testp2p "test/simple01/main.cpp" )
set(MAIN_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(testp2p PUBLIC ${MAIN_INCLUDE_PATH})
target_compile_features(testp2p PRIVATE cxx_std_17)


