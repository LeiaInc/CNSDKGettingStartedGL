#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libzmq-static" for configuration "RelWithDebInfo"
set_property(TARGET libzmq-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(libzmq-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C;CXX;RC"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libzmq-v142-mt-s-4_3_4.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libzmq-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_libzmq-static "${_IMPORT_PREFIX}/lib/libzmq-v142-mt-s-4_3_4.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
