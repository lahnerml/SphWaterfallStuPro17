add_executable(
    SphWaterfall 
        "${CMAKE_CURRENT_LIST_DIR}/SphWaterfall.cpp"
)


include("${CMAKE_CURRENT_LIST_DIR}/cui/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/data/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/export/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/geometry/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/particleGen/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/simulation/CMakeLists.txt")
#include("${CMAKE_CURRENT_LIST_DIR}/visualization/CMakeLists.txt")