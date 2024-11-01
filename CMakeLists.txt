cmake_minimum_required(VERSION 3.14)
project(NEUTvectReader LANGUAGES CXX)

#Use the compilers found in the path
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

find_package(ROOT CONFIG REQUIRED)
find_package(NEUT REQUIRED)

## Includes
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${NEUT_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(basic_reader src/basic_reader.cc)

target_link_libraries(basic_reader PUBLIC stdc++fs
                                            ROOT::Core
                                            ROOT::Tree
                                            NEUT::IO)

add_executable(TotXSPlot src/TotXSPlot.cpp)

target_link_libraries(TotXSPlot PUBLIC stdc++fs
                                            ROOT::Core
                                            ROOT::Tree
                                            NEUT::IO)
