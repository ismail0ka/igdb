add_executable(igdb igdb.cpp)
target_link_libraries(igdb PRIVATE igdb::libigdb PkgConfig::readline)

include(GNUInstallDirs)
install(
    TARGETS igdb
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)