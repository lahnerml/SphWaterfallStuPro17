cmake_minimum_required(VERSION 2.6)
project(SphWaterfall C CXX)

find_package(MPI REQUIRED)
# add this line only when you are using openmpi which has a different c++ bindings
add_definitions(-DOMPI_SKIP_MPICXX)

# uncomment for linux
if(UNIX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lstdc++ -lm")
endif(UNIX)

include("SphWaterfall/src/CMakeLists.txt")

target_link_libraries(SphWaterfall
  PRIVATE
  ${MPI_C_LIBRARIES})

target_include_directories(SphWaterfall
  PRIVATE
  ${MPI_C_INCLUDE_PATH}) 

