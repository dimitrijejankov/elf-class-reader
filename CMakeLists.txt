cmake_minimum_required(VERSION 3.9)
project(testingClassStuff)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")

find_package(LibElf REQUIRED)
find_package(LibDwarf REQUIRED)

# find the boost package
FIND_PACKAGE(Boost COMPONENTS regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(testingClassStuff main.cpp SymbolReader.h SymbolReader.cc)
target_link_libraries(testingClassStuff ${CMAKE_DL_LIBS} ${LIBELF_LIBRARIES} ${LIBDWARF_LIBRARIES} ${Boost_LIBRARIES})

add_library(fancyClass SHARED MyFancyClass.cc MyFancyClass.h)