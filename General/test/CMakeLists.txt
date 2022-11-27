################################################################################
### Setup
################################################################################


################################################################################
### Helpers
################################################################################

function(add_pilg_test executable_subject_path executable_subject)

    set(target_name "pilg_${executable_subject}")    

    file(GLOB_RECURSE TEST_SRCs "${CMAKE_CURRENT_SOURCE_DIR}/${executable_subject_path}/${executable_subject}.cpp")

    add_executable("${target_name}"
                   ${TEST_SRCs})

    target_include_directories("${target_name}"
                               PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>"
                                      "$<INSTALL_INTERFACE:>")

    target_link_libraries("${target_name}"
                          PRIVATE pilg
                                  specific_boost_dependencies
                                  threads) # For now, link with everything we may need

    ########################################
    ### C/C++ definitions
    ########################################


    ########################################
    ### Compile flags
    ########################################

    set_target_properties("${target_name}" PROPERTIES CXX_EXTENSIONS OFF)

    set_property(TARGET "${target_name}" PROPERTY CXX_STANDARD 14)
    
    # Enable link time optimisation
    if(is_lto_avalable)
        set_target_properties("${target_name}" PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()


    ########################################
    ### Boiler plate
    ########################################

    message(STATUS "Adding test ${target_name}")
    add_test(NAME "${target_name}"
                 # The ctest file is in the build tree's root folder but will call executable inside test
                 # and forwarding it's own working directory to the process. If enable_testing was
                 # in this file instead of the "main" cmake file, the ctest file would be in build/tests
                 # and the tests would be broken because the tests rely on working directory to load files for instance.
                 WORKING_DIRECTORY "${PILG_TEST_WORKDIR}" # dont change CWD
                 COMMAND "${target_name}" "in_pipeline")
endfunction()


################################################################################
### Tests
################################################################################

################################ bn  uggler ####################################

add_pilg_test("bn" "bn_juggler")

########################### bn bayesiannetwork #################################

add_pilg_test("bn" "bn_bayesiannetwork")

############################# pdag pdag ########################################

add_pilg_test("pdag" "pdag_pdag")

########################## wgraph wgraph #######################################

add_pilg_test("wgraph" "wgraph_wgraph")

########################## structurelearning mwst_bic ##########################

add_pilg_test("structurelearning" "structurelearning_mwst_bic")

################################################################################

add_pilg_test("creating_bn" "create_bn")

################################################################################

add_pilg_test("summary" "test_summary")

################################################################################

add_pilg_test("save_as_xml" "test_save_as_xml")

################################################################################

add_pilg_test("bn_from_xml" "test_bn_from_xml")

################################################################################

add_pilg_test("generate_csvData" "test_generate_csvData")

################################################################################

add_pilg_test("learnParameters" "test_learnParameters")

################################################################################

add_pilg_test("learnParameters" "test_learnParameters_incre")

################################################################################

add_pilg_test("learnParameters" "test_learnParameters_with_learnObj")

################################################################################

add_pilg_test("learnParameters" "test_learnParameters_incre_with_learnObj")

################################################################################

#add_pilg_test("structurelearning" "test_cache")
