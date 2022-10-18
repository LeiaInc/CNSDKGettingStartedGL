include(CMakeFindDependencyMacro)
if("OFF")
  find_dependency(Threads)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/simdjsonTargets.cmake")
