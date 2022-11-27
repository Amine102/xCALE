# TODO(Etienne M):
# - Namespaces

cmake_minimum_required (VERSION 3.16)

########################################
### Default build type
########################################

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("######## Setting build type to '${default_build_type}' as none was specified")
    message("######## Setting build type to '${default_build_type}' as none was specified")

    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build" FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message("######## Build type is '${CMAKE_BUILD_TYPE}', config types are '${CMAKE_CONFIGURATION_TYPES}'")
message("######## Build type is '${CMAKE_BUILD_TYPE}', config types are '${CMAKE_CONFIGURATION_TYPES}'")


########################################
### Project
########################################

project(Pilgrim-general
        VERSION 0.1.0
        LANGUAGES CXX)


################################################################################
### Setup
################################################################################

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(PILG_IS_MASTER_PROJECT TRUE)
endif()

include(CheckIPOSupported)
include(CMakePackageConfigHelpers)
include(InstallRequiredSystemLibraries)
include(FetchContent)
include(GNUInstallDirs)
include(CTest)

find_package(Git)
find_package(Doxygen)


################################################################################
### Option
################################################################################

option(PILG_BUILD_TEST                "Compile tests." "${BUILD_TESTING}")  # I realy want that to be off by default, but for the typical user of pilgrim it'll make things more compicated
option(PILG_BUILD_USECASE             "Compile usecases." "${BUILD_TESTING}")
option(PILG_GIT_SUBMODULE             "Update submodules." ON)
option(PILG_PROBT_SUBMODULE           "Use the probt submodule (You dont want to use the submodule when creating a package !)." OFF)
option(PILG_POSITION_INDEPENDENT_CODE "If you plan to link pilg to a shared library you probably want this option !." OFF) # Off be default because of a code bloat/extra indirection # slow
option(PILG_FIXED_PACKAGE_NAME        "Use a fixed package name (__package__.<GENERATOR>)." OFF)
option(PILG_NO_ASAN                   "Disable the address sanitizer on Debug and RelWithDebInfo builds." ON) # This is used in the CI to not fail the test because of all the memory leaks in both probt and pilgrim. This is on by default due to the large number of leaks making the tool useless to find new leaks !
set(PILG_TEST_WORKDIR                 "." CACHE STRING "Define the working directory for the tests.") # Usefull in the CI to reference the benchmark files correctly


################################################################################
### Compiler arguments
################################################################################

########################################
### Compiler options
########################################

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang") # Intel not realy supported..
    if(WIN32)
        # There is some problem with the windows crt and clang sanitizer
        # -fsanitize=address dont work correctly in clang + windows situations,  AddressSanitizer: odr-violation in '<string literal>' C:\...\MSVC\14.27.29110\include\xlocnum:1337:99
    else()
        if(NOT PILG_NO_ASAN)
            set(LOCAL_FLAGS_FSANITIZE_ADDR -fsanitize=address ${LOCAL_FLAGS_ASAN_IS_SHARED}) # -fsanitize=cfi -fsanitize=undefined
        endif()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Not supported, it's a hassle
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(LOCAL_FLAGS_COVERAGE --coverage)
    endif()

    set(LOCAL_FLAGS_DNDEBUG -DNDEBUG) # dont use add_compile_definitions because it depends on CMAKE_BUILD_TYPE

    set(LOCAL_FLAGS_STRICT  -Wstrict-aliasing
                            -fstrict-aliasing
                            -Wunreachable-code
                            -Wcast-align
                            -Wcast-qual
                            -Wctor-dtor-privacy
                            -Wdisabled-optimization
                            -Wformat=2
                            -Winit-self 
                            -Wmissing-include-dirs
                            -Wold-style-cast
                            -Woverloaded-virtual
                            -Wredundant-decls
                            -Wshadow
                            -Wsign-promo
                            -Wswitch-default
                            -Wundef
                            -Wdouble-promotion
                            -Wsuggest-override
                            -Wswitch-enum
                            -fdiagnostics-show-option
                            -Wunreachable-code
                            -Wtautological-compare)

    # The flags to set for the compiler are composed of LOCAL_FLAGS_BASE, LOCAL_FLAGS_(DEBUG|MINSIZEREL|RELEASE|RELWITHDEBINFO) and LOCAL_FLAGS_EXTRA

    set(LOCAL_FLAGS_BASE -Wall
                         -Wextra
                         -pedantic
                         ${LOCAL_FLAGS_STRICT})

    set(LOCAL_FLAGS_DEBUG          -g ${LOCAL_FLAGS_FSANITIZE_ADDR} -fno-omit-frame-pointer ${LOCAL_FLAGS_COVERAGE})
    set(LOCAL_FLAGS_MINSIZEREL     -Os -fno-math-errno ${LOCAL_FLAGS_DNDEBUG})
    set(LOCAL_FLAGS_RELEASE        -O3 -fno-math-errno ${LOCAL_FLAGS_DNDEBUG})
    set(LOCAL_FLAGS_RELWITHDEBINFO -O3 -fno-math-errno -g ${LOCAL_FLAGS_FSANITIZE_ADDR} -fno-omit-frame-pointer)

    # Feel free to add the following flags to LOCAL_FLAGS_EXTRA
    set(LOCAL_FLAGS_EXTRA)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    ####################
    # NOTICE ME
    # MSVC is not a great compiler as been "superseded" by ninja/llvm on Windows.
    # You may still use it for debuging or just to edit code but please, please DONT build your release build with MSVC.
    ####################

    set(LOCAL_FLAGS_DNDEBUG /DNDEBUG)
    
    set(LOCAL_FLAGS_DEBUG_INFO_IN_PE    "/Z7")  # The /Z7 option produces object files that also contain full symbolic debugging information for use with the debugger. These object files and the built executable can be substantially larger than files that have no debugging information. The symbolic debugging information includes the names and types of variables, as well as functions and line numbers. No PDB file is produced. 
    set(LOCAL_FLAGS_DEBUG_INFO_IN_PDB   "/Zi")  # The /Zi option produces a separate PDB file that contains all the symbolic debugging information for use with the debugger. The debugging information is not included in the object files or executable, which makes them much smaller.

    # The flags to set for the compiler are composed of LOCAL_FLAGS_BASE, LOCAL_FLAGS_(DEBUG|MINSIZEREL|RELEASE|RELWITHDEBINFO) and LOCAL_FLAGS_EXTRA

    set(LOCAL_FLAGS_BASE /W4
                         /MP
                         /sdl
                         /EHsc)  # /W0, /W1, /W2, /W3, /W4 /Wall Sets output warning level.

    set(LOCAL_FLAGS_DEBUG          ${LOCAL_FLAGS_DEBUG_INFO_IN_PDB}) # /DEBUG:FULL https://docs.microsoft.com/en-us/cpp/build/reference/debug-generate-debug-info?view=vs-2019
    set(LOCAL_FLAGS_MINSIZEREL     /O1 /GS- ${LOCAL_FLAGS_DNDEBUG}) # /O1 (Minimize Size)	/Og /Os /Oy /Ob2 /GF /Gy     these opti makes me loose perf: /Ob3 /LTCG /GL /Gv
    set(LOCAL_FLAGS_RELEASE        /O2 /GS- ${LOCAL_FLAGS_DNDEBUG}) # /O2 (Maximize Speed)	/Og /Oi /Ot /Oy /Ob2 /GF /Gy these opti makes me loose perf: /Ob3 /LTCG /GL /Gv
    set(LOCAL_FLAGS_RELWITHDEBINFO /O2 /GS- ${LOCAL_FLAGS_DEBUG_INFO_IN_PDB})

    # Feel free to add the following flags to CMAKE_CXX_FLAGS
    set(LOCAL_FLAGS_EXTRA /MP)# Multiprocessor/parallel compilation does not seem to be used (mp by default). DOES NOT WORK FOR LINKING/building executable !!
endif()

set(LOCAL_FLAGS ${LOCAL_FLAGS_BASE}
                ${LOCAL_FLAGS}
                ${LOCAL_FLAGS_EXTRA}
                # I branch on config which seems to be set according to CMAKE_BUILD_TYPE if set (make/ninja etc) or
                # on CONFIG (set for multiconfig builder)
                $<$<CONFIG:DEBUG>:${LOCAL_FLAGS_DEBUG}>
                $<$<CONFIG:MINSIZEREL>:${LOCAL_FLAGS_MINSIZEREL}>
                $<$<CONFIG:RELEASE>:${LOCAL_FLAGS_RELEASE}>
                $<$<CONFIG:RELWITHDEBINFO>:${LOCAL_FLAGS_RELWITHDEBINFO}>)


################################################################################
### Dependencies
################################################################################

if(PILG_GIT_SUBMODULE)
    if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
        message(STATUS "Updating the submodules.")
        execute_process(COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive --quiet
                        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                        RESULT_VARIABLE GIT_SUBMODULE_RESULT)
        if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(STATUS "git submodule update --init --recursive --quiet failed with ${GIT_SUBMODULE_RESULT}, please checkout the submodules")
        endif()
    endif()
endif()

# Maybe I shoud explicitly list the dependencies clarity's sake and also because if we add a dependency, cmake 
# wont know that it has been added !
# 
file(GLOB dependencies "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/*")

foreach(a_dependency ${dependencies})
    if(IS_DIRECTORY "${a_dependency}")
        message(STATUS "Adding dependency ${a_dependency}")
        include("${a_dependency}/lib.cmake")
    endif()
endforeach()


################################################################################
### PILG lib
################################################################################

add_library(pilg # calling it general is too general
            STATIC) # Not planing on making a shared lib

add_library(pilgrim::pilg ALIAS pilg)

set(PILG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PILG_HDR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(PILG_SRCs "${PILG_SRC_DIR}/pilgrim/general/pmSlidingWindow.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/random/PMMixed.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmBayesianNetwork.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmFrequencyCounter.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmCSVDataSet.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/log.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/plBayesianNetworkFromXDSLfile.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmUtility.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmDynamicBayesianNetwork.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmCSVdynDataSet.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmClassifier.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmClassifierSingleBN.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmClassifierMultinet.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmWGraph.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pmPDAG.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/pm15dBN.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMMHC_T.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMMPC_T.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMMPCbar_T.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoGS_T.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoGS.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoILPBeta.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMMHC.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMMPC.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMMPCbar.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoMWST.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoTAN.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/Algorithm.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/Algorithm_T.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/algorithms/AlgoDynamic.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/cache.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/computeMI.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/DecomposableScore.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/indeptest.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/pmMI.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/Score.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/ScoreAIC.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/ScoreBDeu.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/ScoreBIC.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/ScoreMDL.cpp"
              "${PILG_SRC_DIR}/pilgrim/general/scores/ScoreQNML.cpp")

# GLOBing file is not considered good practice.
# file(GLOB_RECURSE PILG_SRCs "${PILG_SRC_DIR}/*.cpp")
file(GLOB_RECURSE PILG_HDRs "${PILG_HDR_DIR}/*.h"
                            "${PILG_HDR_DIR}/*.inl")
 
# TODO(Etienne M): Fix that, it seems to be related to 
# https://saas.ls2n.fr/pilgrim/ pmDraw depends on graphviz. The old cmake code
# use to check if the graphviz headers were found in 
if(EXISTS "${LIB_DIR}/include/graphviz/gvc.h")
    set(PILG_SRCs "${PILG_SRCs} ${PILG_SRC_DIR}/pilgrim/general/pmDraw.cpp")
endif()


source_group(sources FILES ${PILG_SRCs})
source_group(include FILES ${PILG_HDRs})


target_sources(pilg
               PRIVATE ${PILG_SRCs}
                       ${PILG_HDRs})

target_include_directories(pilg
                           PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                  "$<INSTALL_INTERFACE:include>")

target_link_libraries(pilg
                      PUBLIC "$<BUILD_INTERFACE:$<IF:$<BOOL:${PILG_PROBT_SUBMODULE}>,probt,probt::probt>>"
                             "$<INSTALL_INTERFACE:probt::probt>"
                      PUBLIC specific_boost_dependencies)


########################################
### Compile flags
########################################

set_target_properties(pilg PROPERTIES CXX_EXTENSIONS OFF)

set_property(TARGET pilg PROPERTY CXX_STANDARD 17)

# Add base line flags plus the ones needed depending on the build type
target_compile_options(pilg
                       PRIVATE ${LOCAL_FLAGS})

# Dirty, tell the linker the same thing to avoid undef ref, mostly usefull when linking the lib itself against an exe 
# or else. This is why its PUBLIC, this way the options are transitively passed for link time.
# That'll leak into the exported cmake package interface and as such it may be better to not
# use generator expression which endup in the Targets file !
target_link_options(pilg
                    PUBLIC ${LOCAL_FLAGS})
                       
# Enable link time optimisation
check_ipo_supported(RESULT is_lto_avalable)

if(is_lto_avalable AND NOT CMAKE_BUILD_TYPE MATCHES Debug)
    # In debug mode, this flags -flto and the like, produce very large obj files that
    # the university gitlab cant handle due to pipeline size restriction (even after heavy 7z compression)
    set_target_properties(pilg 
                          PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set_target_properties(pilg 
                      PROPERTIES POSITION_INDEPENDENT_CODE ${PILG_POSITION_INDEPENDENT_CODE})


########################################
### Installation rules
########################################

if(PILG_IS_MASTER_PROJECT)
    # Uncommenting this line will set the installdirectory to this project's root
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/pilg")

    # install does not support generator expression
    if(PILG_PROBT_SUBMODULE)
        set(SHOULD_EXPORT_PROBT "probt")
    endif()

    install(TARGETS  pilg ${SHOULD_EXPORT_PROBT} specific_boost_dependencies
            EXPORT   pilgTargets
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT pilg_Runtime
            LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT pilg_Runtime NAMELINK_COMPONENT pilg_Development
            ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT pilg_Development)

    install(DIRECTORY   "${CMAKE_CURRENT_SOURCE_DIR}/include"
            DESTINATION ".")

    write_basic_package_version_file(pilgConfigVersion.cmake
                                    VERSION "${PACKAGE_VERSION}"
                                    COMPATIBILITY SameMajorVersion)

    install(EXPORT      pilgTargets 
            FILE        pilgTargets.cmake
            NAMESPACE   pilgrim::
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pilg")

    configure_file(pilgConfig.cmake.in pilgConfig.cmake @ONLY)
    install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/pilgConfig.cmake"
                        "${CMAKE_CURRENT_BINARY_DIR}/pilgConfigVersion.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pilg")
endif()


########################################
### Packaging rules
########################################

if(PILG_IS_MASTER_PROJECT)
    set(CPACK_PACKAGE_NAME          "libpilg-dev")
    set(CPACK_PACKAGE_DESCRIPTION   "ProbabIListic Graphical and RelatIonal Models")
    set(CPACK_PACKAGE_HOMEPAGE_URL  "https://www.ls2n.fr/equipe/duke/")
    set(CPACK_PACKAGE_VENDOR        "LS2N")
    set(CPACK_PACKAGE_CONTACT       "LS2N")
    if(PILG_FIXED_PACKAGE_NAME)
        set(CPACK_PACKAGE_FILE_NAME "__package__") # All packages are named __package__.GENERATOR
    endif()

    # set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE") # TODO(Etienne M): License

    if(UNIX)
        set(AVAILABLE_CPACK_GENERATOR "DEB;")
    elseif(WIN32)
        # TODO(Etienne M): nsis ?
    endif()

    set(CPACK_GENERATOR "${AVAILABLE_CPACK_GENERATOR}TGZ")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libprobt-dev, libboost-system-dev, libboost-filesystem-dev") # TODO(Etienne M): libgraphviz-dev, libpugixml-dev

    # This include needs to be here, else it does not take the CPACK_* variables into account
    include(CPack)
endif()


################################################################################
### Tests and usecases
################################################################################

if(PILG_BUILD_TEST AND (PILG_IS_MASTER_PROJECT OR BUILD_TESTING))
    add_subdirectory(test)
endif()

if(PILG_BUILD_USECASE AND PILG_IS_MASTER_PROJECT)
    add_subdirectory(usecases)
endif()


################################################################################
### Documentation
################################################################################

message("Add Doxygen target if found (value = ${DOXYGEN_FOUND}")
if(DOXYGEN_FOUND)
    set(PILG_DOC_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" 
                   "${PILG_DOC_DIR}/Doxyfile" @ONLY)
    add_custom_target("doc"
                      COMMAND "${DOXYGEN_EXECUTABLE}"
                      WORKING_DIRECTORY "${PILG_DOC_DIR}"
                      COMMENT "Builds Doxygen documentation"
                      SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
endif()
