# Look for the necessary header
find_path(probt_INCLUDE_DIR NAMES "probt/plVersion.h")
mark_as_advanced(probt_INCLUDE_DIR)

# Look for the necessary library
find_library(probt_LIBRARY NAMES probt)
mark_as_advanced(probt_LIBRARY)

# Extract version information from the header file
if(probt_INCLUDE_DIR)
    # probt is not correctly made, it expects his header as #include "plVersion.h"
    # and not #include "probt/plVersion.h"
    set(probt_INCLUDE_DIR "${probt_INCLUDE_DIR}/probt") 
    
    file(READ "${probt_INCLUDE_DIR}/plVersion.h" version_file)

    string(REGEX MATCH "PROBT_MAJOR +([0-9]+)" _ ${version_file})
    set(_version_line_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "PROBT_MINOR +([0-9]+)" _ ${version_file})
    set(_version_line_minor ${CMAKE_MATCH_1})

    string(REGEX MATCH "PROBT_REVISION +([0-9]+)" _ ${version_file})
    set(_version_line_revision ${CMAKE_MATCH_1})

    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
           probt_VERSION "${_version_line_major}.${_version_line_minor}.${_version_line_revision}")

    unset(_version_line_major)
    unset(_version_line_minor)
    unset(_version_line_revision)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(probt
    REQUIRED_VARS probt_INCLUDE_DIR probt_LIBRARY
    VERSION_VAR probt_VERSION)

# Create the imported target
if(probt_FOUND)
    set(probt_INCLUDE_DIRS ${probt_INCLUDE_DIR})
    set(probt_LIBRARIES ${probt_LIBRARY})
    if(NOT TARGET probt::probt)
        add_library(probt::probt SHARED IMPORTED)
        set_target_properties(probt::probt PROPERTIES
            IMPORTED_LOCATION             "${probt_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${probt_INCLUDE_DIR}")
    endif()
endif()
