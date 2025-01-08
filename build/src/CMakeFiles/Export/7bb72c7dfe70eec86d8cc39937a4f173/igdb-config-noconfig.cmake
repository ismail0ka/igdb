#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "igdb::libigdb" for configuration ""
set_property(TARGET igdb::libigdb APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(igdb::libigdb PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libigdb.a"
  )

list(APPEND _cmake_import_check_targets igdb::libigdb )
list(APPEND _cmake_import_check_files_for_igdb::libigdb "${_IMPORT_PREFIX}/lib/libigdb.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
