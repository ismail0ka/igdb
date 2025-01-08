add_library(libigdb libigdb.cpp)
add_library(igdb::libigdb ALIAS libigdb)

set_target_properties(
    libigdb
    PROPERTIES OUTPUT_NAME igdb
)

target_compile_features(libigdb PUBLIC cxx_std_23)

target_include_directories(
    libigdb
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/include
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

include(GNUInstallDirs)
install(TARGETS libigdb
    EXPORT igdb-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT igdb-targets
    FILE igdb-config.cmake
    NAMESPACE igdb::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igdb
)