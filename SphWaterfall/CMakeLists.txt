cmake_minimum_required(VERSION 2.6)
project(SphWaterfall)

if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(MPI REQUIRED)
# add this line only when you are using openmpi which has a different c++ bindings
add_definitions(-DOMPI_SKIP_MPICXX)

include("SphWaterfall/src/CMakeLists.txt")

target_link_libraries(SphWaterfall
  PRIVATE
  ${MPI_C_LIBRARIES})

target_include_directories(SphWaterfall
  PRIVATE
  ${MPI_C_INCLUDE_PATH}) 
