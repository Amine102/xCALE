cmake_minimum_required (VERSION 3.16)

project(Probt
        VERSION 3.0.0 # It's actually 3.1.0 and 3.0.0.. thanks probt
        LANGUAGES CXX)


################################################################################
### Setup
################################################################################

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(PROBT_IS_MASTER_PROJECT TRUE)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)


################################################################################
### Option
################################################################################

option(PROBT_CI_PACKAGE_NAME "Use a fixed package name." OFF)


################################################################################
### Compiler arguments
################################################################################

########################################
### Compiler options
########################################

########################################
### Default build type
########################################

################################################################################
### Dependencies
################################################################################

################################################################################
### probt lib
################################################################################

if(WIN32)
    # TODO(Etienne M): Handle debug

    set(PROBT_VERSION "3.0.0") # We dont have the 3.1.0 on windows
    set(PROBT_LIB_PATH "lib/${PROBT_VERSION}.amd64.windows.shared.release")

    set(PROBT_LIB "probt-vc120.dll")
    set(PROBT_IMPLIB "probt-vc120.lib")
elseif(UNIX)
    set(PROBT_VERSION "3.1.0")
    set(PROBT_LIB_PATH "lib/${PROBT_VERSION}.amd64.linux.shared.release")
    
    set(PROBT_LIB "libprobt.so")
    # We need libboost_serialization.so.1.58.0 TODO(Etienne M): This should be a system dependency and not in this repo
endif()

set(PROBT_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROBT_VERSION}.include")

# GLOBing file is not considered good practice.
file(GLOB_RECURSE PROBT_HEADERS "${PROBT_HEADER_DIR}/*.h"
                                "${PROBT_HEADER_DIR}/*.inl")

source_group(include FILES ${PROBT_HEADERS})

# This below was an attempt at using IMPORTED libs. It wasnt great.. by may be a more
# correct solution if working.

# add_library(probt
#             SHARED IMPORTED GLOBAL)

# # add_library(probt::probt ALIAS probt)

# set_target_properties(probt
#                       PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROBT_HEADER_DIR}")

# set_property(TARGET probt 
#              PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${PROBT_LIB_PATH}/${PROBT_LIB}")

# if(WIN32)
#     set_property(TARGET probt
#                  PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/${PROBT_LIB_PATH}/${PROBT_IMPLIB}")
# endif()

add_library(probt
            INTERFACE)

add_library(probt::probt ALIAS probt)

target_include_directories(probt
                           INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROBT_VERSION}.include>"
                                     "$<INSTALL_INTERFACE:include/probt>") # Im not even installnig the target but I still need this generator -__-

target_link_libraries(probt
                      INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/${PROBT_LIB_PATH}/${PROBT_LIB}")

if(WIN32)
    target_link_libraries(probt
                          INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/${PROBT_LIB_PATH}/${PROBT_IMPLIB}")
endif()


########################################
### Compile flags
########################################

########################################
### Installation rules
########################################

if(PROBT_IS_MASTER_PROJECT)
    # Uncommenting this line will set the installdirectory to this project's root
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/probt")

    install(DIRECTORY   "${PROBT_LIB_PATH}/"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}")

    install(DIRECTORY   "${PROBT_HEADER_DIR}/"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/probt")

    write_basic_package_version_file(probtConfigVersion.cmake
                                    VERSION "${PROBT_VERSION}"
                                    COMPATIBILITY SameMajorVersion)

    configure_file(probtConfig.cmake.in probtConfig.cmake @ONLY)
    install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/probtConfig.cmake"
                        "${CMAKE_CURRENT_BINARY_DIR}/probtConfigVersion.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/probt")
    
    install(FILES       "${CMAKE_CURRENT_SOURCE_DIR}/Findprobt.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/probt/find_modules")
endif()


########################################
### Packaging rules
########################################

if(PROBT_IS_MASTER_PROJECT)
    set(CPACK_PACKAGE_NAME          "libprobt-dev")
    set(CPACK_PACKAGE_HOMEPAGE_URL  "https://www.probayes.com/")
    set(CPACK_PACKAGE_VENDOR        "Probayes")
    set(CPACK_PACKAGE_CONTACT       "Probayes")
    set(CPACK_PACKAGE_VERSION       "${PROBT_VERSION}")
    if(PROBT_CI_PACKAGE_NAME)
        set(CPACK_PACKAGE_FILE_NAME "__package__") # All packages are named __package__.GENERATOR
    endif()

    # set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE") # TODO(Etienne M): License
    
    if(UNIX)
        set(AVAILABLE_CPACK_GENERATOR "DEB;")
    elseif(WIN32)
        # TODO(Etienne M): nsis ?
    endif()

    set(CPACK_GENERATOR "${AVAILABLE_CPACK_GENERATOR}TGZ")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "") # TODO(Etienne M): What are the exact dependencies

    # This include needs to be here, else it does not take the CPACK_* variables into account
    include(CPack)
endif()


################################################################################
### Tests and usecases
################################################################################

################################################################################
### Documentation
################################################################################
