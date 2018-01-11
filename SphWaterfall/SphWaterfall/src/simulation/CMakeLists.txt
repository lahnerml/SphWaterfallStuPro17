target_sources(SphWaterfall 
    PUBLIC    
		"${CMAKE_CURRENT_LIST_DIR}/DomainDecomposer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/DomainDecomposer.h"
		"${CMAKE_CURRENT_LIST_DIR}/ISphKernel.h"
		"${CMAKE_CURRENT_LIST_DIR}/ISphNeighbourSearch.h"
		"${CMAKE_CURRENT_LIST_DIR}/ParticleDomain.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/ParticleDomain.h"
		"${CMAKE_CURRENT_LIST_DIR}/SphManager.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/SphManager.h"
		"${CMAKE_CURRENT_LIST_DIR}/SphNeighbourSearch.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/SphNeighbourSearch.h"
		"${CMAKE_CURRENT_LIST_DIR}/WendlandKernel.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/WendlandKernel.h"

)