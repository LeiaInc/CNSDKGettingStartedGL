#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Tracy::TracyClient" for configuration "RelWithDebInfo"
set_property(TARGET Tracy::TracyClient APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Tracy::TracyClient PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/TracyClient.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/TracyClient.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Tracy::TracyClient )
list(APPEND _IMPORT_CHECK_FILES_FOR_Tracy::TracyClient "${_IMPORT_PREFIX}/lib/TracyClient.lib" "${_IMPORT_PREFIX}/bin/TracyClient.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
