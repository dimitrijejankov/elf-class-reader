cmake_minimum_required(VERSION 3.9)
project(testingClassStuff)

# set clang as the default compiler
set(CMAKE_C_COMPILER "clang-6.0")
set(CMAKE_CXX_COMPILER "clang++-6.0")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")

set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)

find_package(LibElf REQUIRED)
find_package(LibDwarf REQUIRED)

# find the boost package
FIND_PACKAGE(Boost COMPONENTS regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(testingClassStuff main.cpp SymbolReader.h SymbolReader.cc
                                          Reflection.cc Reflection.h
                                          ClassReflection.h ClassReflection.cc
                                          MethodReflection.h MethodReflection.cc)

target_link_libraries(testingClassStuff ${CMAKE_DL_LIBS} ${LIBELF_LIBRARIES} ${LIBDWARF_LIBRARIES} ${Boost_LIBRARIES})

add_library(fancyClass SHARED MyFancyClass.cc MyFancyClass.h)