cmake_minimum_required(VERSION 2.6)
project(SphWaterfall)

include("SphWaterfall/src/CMakeLists.txt")

# find mpi
find_package (MPI REQUIRED)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${MPI_INCLUDE_PATH})