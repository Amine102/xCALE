################################################################################
### Setup
################################################################################


################################################################################
### Helpers
################################################################################

function(add_pilg_usecase target_name test_sources)

    add_executable("${target_name}"
                   ${test_sources})

    target_link_libraries("${target_name}"
                          PRIVATE pilg
                                  specific_boost_dependencies) # For now, link with everything we may need

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

endfunction()


################################################################################
### Usecases
################################################################################

set(pilg_usecase_src "main.cpp"
                     "BN/TestsForIMMPC/immhc.cpp"
                     "BN/TestsForIMMPC/mmhc.cpp"
                     "BN/TestsForIMMPC/IncrementalFC_test.cpp"
                     "BN/structureLearning/Asia_MMHC_structureLearning.cpp"
                     "BN/structureLearning/AsiaStructureLearningComparisons.cpp"
                     "BN/structureLearning/Asia_MWST_structureLearning.cpp"
                     "BN/structureLearning/Asia_GS_structureLearningWithMwstInit.cpp"
                     "BN/structureLearning/Alarm_GS_structureLearning.cpp"
                     "BN/Alarm/LoadAlarmAndGenerateData.cpp"
                     "BN/HailFinder2-5/LoadHailFinderAndGenerateData.cpp"
                     "BN/structureLearning/Asia_GS_structureLearningWithRandomInit.cpp"
                     "BN/Asia/LoadAsiaAndGenerateData.cpp"
                     "BN/DummyUseCase/non_stationnaireSystemExampleDummy.cpp"
                     "BN/DummyUseCase/TestingLearnIncremental.cpp"
                     "BN/DummyUseCase/learnDummyIncremental.cpp"
                     "BN/DummyUseCase/learnDummy.cpp"
                     "BN/DummyUseCase/HelperFunctions.cpp"
                     "BN/DummyUseCase/createDummy3.cpp"
                     "BN/DummyUseCase/createDummy2.cpp"
                     "BN/DummyUseCase/createDummy1.cpp"
                     "BN/EyeColorGene/createEyeColorGeneBN.cpp"
                     "BN/EyeColorGene/createEyeColorGeneBN_maxFn.cpp"
                     "BN/EyeColorGene/queryEyeColorGeneBN.cpp"
		             "Deprecated/createAndExportBNSpam.cpp"
                     "Deprecated/createBN.cpp"
                     "Deprecated/createBNJuggler.cpp"
                     "Deprecated/createLabeledSpamBN.cpp"
                     "Deprecated/learnSpamBN.cpp"
                     "Deprecated/loadBNJuggler.cpp"
                     "Deprecated/loadFromXml.cpp"
                     "Deprecated/noisyMaxProbt.cpp"
                     "Deprecated/noisyMaxSerialize.cpp"
                     "Deprecated/noisyMaxUnserialize.cpp"
                     "Deprecated/testKLDivergence.cpp"
                     "Deprecated/createDynamicBN.cpp"
                     "Deprecated/queryDynamicBN.cpp"
                     "Deprecated/buildWGraph.cpp"
                     "Deprecated/testDBNOptimizedQuery.cpp"
                     "Deprecated/test_15dBN.cpp"
                     "Deprecated/DBN_load.cpp"
                     "Deprecated/DBN_query.cpp"
                     "Deprecated/DBN_unroll.cpp"
                     "Deprecated/DBN_generate_csv_data.cpp"
                     "Deprecated/DBN_get_initial_and_Transition_graphs.cpp"
                     "Deprecated/DBN_get_JointDistribution.cpp"
                     "Deprecated/how_to_create_DBN15.cpp"
                     "Deprecated/DBN15_use_predict.cpp"
                     "Deprecated/DBN15_getOutgoingInterface.cpp"
    		         "Deprecated/testDMMHC.cpp"
    		         "BN/Juggler/createJuggler.cpp"
    		         "BN/Juggler/learnParametersJuggler.cpp"
    		         "BN/Juggler/queryJuggler.cpp"
    		         "BN/Spam/createSpam.cpp"
    		         "BN/Spam/learnParametersSpam.cpp"
    		         "BN/Spam/querySpam.cpp"
    		         "BN/noisyMax/createNoisyMax.cpp"
                     "BN/noisyMax/queryNoisyMax.cpp"
                     "BN/noisyMax/learnParametersNoisyMax.cpp"
                     "BN/noisyMax/Helper_Functions.cpp"
                     "BN/noisyMax/learnParamsNoisyMaxIncremental.cpp"
		             "BN/structureLearning/Dummy_GS_structureLearning.cpp"
                     "BN/Exam/createExam.cpp"
                     "BN/Minesweeper/createMinesweeper.cpp"
                     "Algo/Juggler/learnStructJuggler.cpp"
                     "Algo/DBN/learnStructDBN.cpp"
                     "DBN/createDBN.cpp"
                     "BN/UniAdmissions/createUniAdmissions.cpp"
                     "BN/UniAdmissions/queryUniAdmissions.cpp"
                     "BN/UniAdmissions/learnParametersUniAdmissions.cpp"
                     "Algo/UniAdmissions/learnStructUniAdmissions_GS.cpp"
                     "BN/Marks/createMarks.cpp"
                     "BN/Marks/queryMarks.cpp"
                     "BN/Marks/learnParametersMarks.cpp"
                     "Algo/Marks/learnStructMarks_MWST.cpp"
                     "BN/Weather/createWeather.cpp"
                     "BN/Weather/learnParametersWeather.cpp"
                     "BN/Weather/learnParametersWeatherIncremental.cpp"
                     "BN/ExamenMarks/createExamenMarks.cpp"
                     "BN/ExamenMarks/queryExamenMarks.cpp"
                     "BN/ExamenMarks/learnParametersExamenMarks.cpp"
                     "Algo/ExamenMarks/learnStructExamenMarks.cpp"
                     "BN/NoisyConditionals/createNoisyConditionals.cpp"
                     "BN/NoisyConditionals/testNoisyConditionals.cpp"
                     "BN/Multinet/learnParametersMultinet.cpp"
                     "BN/SingleBN/learnParametersSingleBN.cpp"
                     "BN/Contraception/learnParametersContraception.cpp"
                     "BN/Contraception/createContraception.cpp"
                     "BN/Contraception/createDataContraception.cpp"
                     "BN/Contraception/learnParametersData.cpp"
                     "BN/Abalone/learnParametersAbalone.cpp"
                     "BN/NaiveTan/learnParametersNaiveTan.cpp"
                     "BN/NaiveTan/createNaiveTan.cpp"
	                 "BN/UnsupervisedSpam/createUnsupervisedSpam.cpp"
      	             "BN/UnsupervisedSpam/learnParametersUnsupervisedSpam.cpp"
            	     "BN/FinancialWellBeing/learnParametersFWB.cpp"
                     "BN/FinancialWellBeing/learnStructureFWB_GS.cpp"
                     "BN/FinancialWellBeing/learnStructureFWB_MWST.cpp"
                     "BN/FinancialWellBeing/learnStructureFWB_GSinitMWST.cpp"
                     "DBN/CreateDBNWithLO.cpp"
                     "DBN/QueryDBNWithLO.cpp"
                     "DBN/createDBNidentical.cpp"
                     "DBN/createBKT.cpp"
                     "BN/usecase1_create_BN_and_xml/create_BN_and_xml.cpp"
                     "BN/usecase2_learnParameters/usecase2_generate_data.cpp"
                     "BN/usecase2_learnParameters/simple_learnParameters.cpp"
                     "BN/usecase2_learnParameters/learnParameters_incremental.cpp"
                     "BN/usecase2_learnParameters/learnParameters_with_learnObj.cpp"
                     "BN/usecase2_learnParameters/learnParameters_incremental_with_learnObj.cpp"
                     "BN/usecase3_structure_learning/learnStruct.cpp"
                     "BN/usecase3_structure_learning/learnStruct_constraint.cpp"
                     "BN/usecase3_structure_learning/learnStructMWST.cpp"
                     "DBN/usecase1/create_DBN_and_xml.cpp"
                     "DBN/usecase2_learnParameters/generate_data.cpp"
                     "BN/DAGtoCPDAGexample/DAGtoCPDAGexample.cpp")

add_pilg_usecase(pilg_usecases
                 "${pilg_usecase_src}")
