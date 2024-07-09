
# --- FIND nebulaatom  ---

# Find include dirs
find_path(nebulaatom_INCLUDE_DIRS
	NAME "core/nebula_atom.h"
	PATHS 
		/usr/include
		/usr/include/nebulaatom
		/usr/include/nebulaatom
		/usr/local/include/nebulaatom
		/opt/nebulaatom/nebulaatom/include
		/opt/nebulaatom/include
		/usr/local/nebulaatom/include
		/usr/local/nebulaatom/include/nebulaatom
)

# Find library dirs
find_library(nebulaatom_LIBRARIES
	NAMES nebulaatom
	PATHS /usr/lib
	/usr/lib/nebulaatom
    /usr/lib/x86_64-linux-gnu/nebulaatom
    /usr/local/lib/nebulaatom
    /usr/local/nebulaatom/lib
    /usr/local/nebulaatom/lib/nebulaatom
    /opt/nebulaatom/nebulaatom/lib/nebulaatom
    /opt/nebulaatom/lib/nebulaatom
    /opt/rh/rh-mariadb105/root/usr/lib64
)

# Verify if exists dirs
if(nebulaatom_INCLUDE_DIRS AND nebulaatom_LIBRARIES)
  message(STATUS "nebulaatom Include dir: ${nebulaatom_INCLUDE_DIRS}")
  message(STATUS "nebulaatom client libraries: ${nebulaatom_LIBRARIES}")
elseif(nebulaatom_FIND_REQUIRED)
	message(FATAL_ERROR "Cannot find nebulaatom. Include dir: ${nebulaatom_INCLUDE_DIRS}  library dir: ${nebulaatom_LIBRARIES_DIR}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(nebulaatom
	DEFAULT_MSG
	nebulaatom_LIBRARIES
	nebulaatom_INCLUDE_DIRS
)

# Copy the results to the output variables.
if(NOT TARGET nebulaatom::nebulaatom)
	add_library(nebulaatom::nebulaatom INTERFACE IMPORTED)
	set_target_properties(nebulaatom::nebulaatom PROPERTIES 
		INTERFACE_INCLUDE_DIRECTORIES "${nebulaatom_INCLUDE_DIRS}"
		INTERFACE_LINK_LIBRARIES "${nebulaatom_LIBRARIES}"
	)
else()
	set(nebulaatom_LIBRARIES)
	set(nebulaatom_INCLUDE_DIRS)
endif()

mark_as_advanced(nebulaatom_INCLUDE_DIRS nebulaatom_LIBRARIES)