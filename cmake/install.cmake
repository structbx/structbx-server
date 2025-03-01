
# Install files
include(GNUInstallDirs)
install(
    TARGETS structbx-server
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
	FILES ${PROJECT_SOURCE_DIR}/conf/properties.yaml.template
	DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/structbx/properties.yaml
)
