cmake_minimum_required(VERSION 3.12)
project(DPDensityBased)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 14)

add_executable(DP main.cpp Model/Implementation/Point.cpp Model/Point.h Model/Implementation/Cell.cpp Model/Cell.h Model/Implementation/Cluster.cpp Model/Cluster.h Model/Implementation/Level.cpp Model/Level.h main.h Model/Implementation/DataSet.cpp Model/DataSet.h Model/Implementation/HyperGridBitmap.cpp Model/HyperGridBitmap.h)