
# Install files
include(GNUInstallDirs)

# Create config directory
set(CONFIG_OUTPUT_DIR "${CMAKE_BINARY_DIR}/config_files")
file(MAKE_DIRECTORY "${CONFIG_OUTPUT_DIR}")

## a) properties.yaml
configure_file(
    "${PROJECT_SOURCE_DIR}/conf/properties.yaml.template"
    "${CONFIG_OUTPUT_DIR}/properties.yaml"
    COPYONLY
)
## b) cert.pem
configure_file(
    "${PROJECT_SOURCE_DIR}/conf/cert.pem.template"
    "${CONFIG_OUTPUT_DIR}/cert.pem"
    COPYONLY
)
# c) key.pem
configure_file(
    "${PROJECT_SOURCE_DIR}/conf/key.pem.template"
    "${CONFIG_OUTPUT_DIR}/key.pem"
    COPYONLY
)

# Install files
install(
    TARGETS structbx-server
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    FILES
        "${CONFIG_OUTPUT_DIR}/properties.yaml"
        "${CONFIG_OUTPUT_DIR}/cert.pem"
        "${CONFIG_OUTPUT_DIR}/key.pem"
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/structbx
)