#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CNSDK::leiaSDK-faceTrackingInApp-shared" for configuration "Release"
set_property(TARGET CNSDK::leiaSDK-faceTrackingInApp-shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CNSDK::leiaSDK-faceTrackingInApp-shared PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/leiaSDK-faceTrackingInApp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/leiaSDK-faceTrackingInApp.dll"
  )

list(APPEND _cmake_import_check_targets CNSDK::leiaSDK-faceTrackingInApp-shared )
list(APPEND _cmake_import_check_files_for_CNSDK::leiaSDK-faceTrackingInApp-shared "${_IMPORT_PREFIX}/lib/leiaSDK-faceTrackingInApp.lib" "${_IMPORT_PREFIX}/bin/leiaSDK-faceTrackingInApp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)