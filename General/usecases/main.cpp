/**
  * [Not up-to-date]
  * File : main.cpp
  * Description : Entry point for the usecases.
  * Author : Thomas Vincent
  * Creation Date : 13/04/2015
  */

/**
 * (Quentin Couland)
 * If you wanna add a usecase, please:
 * 0) Include the header file (warning: using a different function name than the filename might produce a linking error)
 * 1) Populate the displayPossibleChoices() at the end of the file
 * 2) Add it to the switch statement
 **/
#include <string>
#include <iostream>

#include "BN/Juggler/createJuggler.h"
#include "BN/Juggler/queryJuggler.h"
#include "BN/Juggler/learnParametersJuggler.h"
#include "BN/Spam/createSpam.h"
#include "BN/Spam/querySpam.h"
#include "BN/Spam/learnParametersSpam.h"
#include "BN/noisyMax/createNoisyMax.h"
#include "BN/noisyMax/queryNoisyMax.h"
#include "BN/noisyMax/learnParametersNoisyMax.h"
#include "BN/Exam/createExam.h"
#include "BN/Minesweeper/createMinesweeper.h"
#include "Algo/Juggler/learnStructJuggler.h"
#include "Algo/DBN/learnStructDBN.h"
#include "DBN/createDBN.h"
#include "BN/EyeColorGene/createEyeColorGeneBN.h"
#include "BN/EyeColorGene/queryEyeColorGeneBN.h"
#include "BN/EyeColorGene/createEyeColorGeneBN_maxFn.h"
#include "BN/noisyMax/learnParamsNoisyMaxIncremental.h"
#include "BN/noisyMax/Helper_Functions.h"
#include "BN/DummyUseCase/createDummy1.h"
#include "BN/DummyUseCase/createDummy2.h"
#include "BN/DummyUseCase/createDummy3.h"
#include "BN/DummyUseCase/learnDummy.h"
#include "BN/DummyUseCase/learnDummyIncremental.h"
#include "BN/DummyUseCase/TestingLearnIncremental.h"
#include "BN/DummyUseCase/non_stationnaireSystemExampleDummy.h"
#include "BN/DummyUseCase/HelperFunctions.h"
#include "BN/UniAdmissions/createUniAdmissions.h"
#include "BN/UniAdmissions/queryUniAdmissions.h"
#include "BN/UniAdmissions/learnParametersUniAdmissions.h"
#include "Algo/UniAdmissions/learnStructUniAdmissions_GS.h"
#include "BN/Marks/createMarks.h"
#include "BN/Marks/queryMarks.h"
#include "BN/Marks/learnParametersMarks.h"
#include "Algo/Marks/learnStructMarks_MWST.h"
#include "BN/Asia/LoadAsiaAndGenerateData.h"
#include "BN/structureLearning/AsiaStructureLearningComparisons.h"
#include "BN/structureLearning/Asia_GS_structureLearningWithRandomInit.h"
#include "BN/structureLearning/Asia_GS_structureLearningWithMwstInit.h"
#include "BN/HailFinder2-5/LoadHailFinderAndGenerateData.h"
#include "BN/Alarm/LoadAlarmAndGenerateData.h"
#include "BN/structureLearning/Alarm_GS_structureLearning.h"
#include "BN/structureLearning/Asia_MWST_structureLearning.h"
#include "BN/structureLearning/Dummy_GS_structureLearning.h"
#include "BN/Weather/createWeather.h"
#include "BN/Weather/learnParametersWeather.h"
#include "BN/Weather/learnParametersWeatherIncremental.h"
#include "BN/ExamenMarks/createExamenMarks.h"
#include "BN/ExamenMarks/queryExamenMarks.h"
#include "BN/ExamenMarks/learnParametersExamenMarks.h"
#include "Algo/ExamenMarks/learnStructExamenMarks.h"
#include "BN/structureLearning/Asia_MMHC_structureLearning.h"
#include "BN/TestsForIMMPC/IncrementalFC_test.h"
#include "BN/TestsForIMMPC/immhc.h"
#include "BN/TestsForIMMPC/mmhc.h"
#include "BN/NoisyConditionals/createNoisyConditionals.h"
#include "BN/NoisyConditionals/testNoisyConditionals.h"
#include "BN/Multinet/learnParametersMultinet.h"
#include "BN/SingleBN/learnParametersSingleBN.h"
#include "BN/Contraception/learnParametersContraception.h"
#include "BN/Contraception/createContraception.h"
#include "BN/Contraception/createDataContraception.h"
#include "BN/Contraception/learnParametersData.h"
#include "BN/Abalone/learnParametersAbalone.h"
#include "BN/NaiveTan/createNaiveTan.h"
#include "BN/NaiveTan/learnParametersNaiveTan.h"
#include "BN/UnsupervisedSpam/createUnsupervisedSpam.h"
#include "BN/UnsupervisedSpam/learnParametersUnsupervisedSpam.h"
#include "BN/FinancialWellBeing/learnParametersFWB.h"
#include "BN/FinancialWellBeing/learnStructureFWB_GS.h"
#include "BN/FinancialWellBeing/learnStructureFWB_MWST.h"
#include "BN/FinancialWellBeing/learnStructureFWB_GSinitMWST.h"
#include "DBN/CreateDBNWithLO.h"
#include "DBN/QueryDBNWithLO.h"
#include "DBN/createDBNidentical.h"
#include "DBN/createBKT.h"
#include "BN/usecase1_create_BN_and_xml/usecase1.h"
#include "BN/usecase2_learnParameters/usecase2.h"
#include "BN/usecase3_structure_learning/learnStruct.h"
#include "DBN/usecase1/usecase1.h"
#include "DBN/usecase2_learnParameters/usecase2.h"
#include "BN/DAGtoCPDAGexample/DAGtoCPDAGexample.h"


using namespace std;
using namespace PILGRIM;

void displayPossibleChoices();

int main(int argc, char **argv)
{
  string rep;

  do {
    displayPossibleChoices();

    unsigned int choice = 0;

    std::cin >> choice;

    switch (choice)
    {
    case 1:
      createJuggler();
      break;
    case 2:
      queryJuggler();
      break;
    case 3:
      learnParametersJuggler();
      break;
    case 4:
      createSpam();
      break;
    case 5:
      querySpam();
      break;
    case 6:
      learnParametersSpam();
      break;
    case 7:
      createNoisyMax();
      break;
    case 8:
      queryNoisyMax();
      break;
    case 9:
      learnParametersNoisyMax();
      break;
    case 10:
      learnParamsNoisyMaxIncremental();
      break;
    case 11:
      Dummy_GS_structureLearning();
      break;
    case 12:
      createExam();
      break;
    case 13:
      createMinesweeper();
      break;
    case 14:
      runBNAlgoGS();
      break;
    case 15:
      NewDBN();
      break;
    case 16:
      runDBNAlgoGS();
      break;
    case 17:
      createEyeColorGeneBN();
      break;
    case 18:
      createEyeColorGeneBN_maxFn();
      break;
    case 19:
      queryEyeColorGeneBN();
      break;
    case 20:
      createDummy1();
      break;
    case 21:
      createDummy2();
      break;
    case 22:
      createDummy3();
      break;
    case 23:
      learnDummy();
      break;
    case 24:
      learnDummyIncremental();
      break;
    case 25:
      TestingLearnIncremental();
      break;
    case 26:
      non_stationnaireSystemExampleDummy();
      break;
    case 27:
      LoadAsiaAndGenerateData();
      break;
    case 28:
      Asia_GS_structureLearningWithRandomInit();
      break;
    case 29:
      Asia_GS_structureLearningWithMwstInit();
      break;
    case 30:
      Asia_MWST_structureLearning();
      break;
    case 31:
      AsiaStructureLearningComparisons();
      break;
    case 32:
      LoadHailFinderAndGenerateData();
      break;
    case 33:
      LoadAlarmAndGenerateData();
      break;
    case 34:
      Alarm_GS_structureLearning();
      break;
    case 35:
      createExamenMarks();
      break;
    case 36:
      queryExamenMarks();
      break;
    case 37:
      learnParametersExamenMarks();
      break;
    case 38:
      runBNAlgoExamenMarks();
      break;
    case 39:
      createUniAdmissions();
      break;
    case 40:
      queryUniAdmissions();
      break;
    case 41:
      learnParametersUniAdmissions();
      break;
    case 42:
      runBNAlgosUniAdmissions_GS();
      break;
    case 43:
      createMarks();
      break;
    case 44:
      queryMarks();
      break;
    case 45:
      learnParametersMarks();
      break;
    case 46:
      runBNAlgosMarks_MWST();
      break;
    case 47:
      createWeather();
      break;
    case 48:
      learnParametersWeather();
      break;
    case 49:
      learnParametersIncrementalWeather();
      break;
    case 50:
      IncrementalFC_test();
      break;
    case 51:
      immhc();
      break;
    case 52:
      mmhc();
      break;
    case 53:
      createNoisyConditionals();
      break;
    case 54:
      testNoisyConditionals();
      break;
    case 55 :
      learnParametersMultinet();
      break;
    case 56 :
      learnParametersSingleBN();
      break;
    case 57:
      learnParametersContraception();
      break;
    case 58 :
      createContraception();
      break;
    case 59 :
      createDataContraception();
      break;
    case 60 :
      learnParametersData();
      break;
    case 61 :
      createNaiveTan();
      break;
    case 62 :
      learnParametersNaiveTan();
      break;
    case 63 :
      learnParametersAbalone();
      break;
    case 64 : createUnsupervisedSpam();
      break;
    case 65 : learnParametersUnsupervisedSpam();
      break;
    case 66 : learnParametersFWB();
      break;
    case 67 : learnStructureFWB_GS();
      break;
    case 68 : learnStructureFWB_MWST();
      break;
    case 69 : learnStructureFWB_GSinitMWST();
      break;
    case 70 : CreateDBNWithLO();
      break;
    case 71: QueryDBNWithLO();
      break;
    case 72: createDBNidentical();
      break;
    case 73: createAndQueryBKT();
      break;
    case 74 : create_BN_and_xml();
      break;
    case 75 : usecase2_dataGen();
      break;
    case 76 : usecase2();
      break;
    case 77 : usecase2_incre();
      break;
    case 78 : usecase2_learnObj();
      break;
    case 79 : usecase2_incre_learnObj();
      break;
    case 80 : usecase3_learnStruct();
      break;
    case 81 : usecase3_learnStruct_constraint();
      break;
    case 82 : usecase3_learnStructMWST();
      break;
    case 83 : create_DBN_and_xml();
      break;
    case 84 : dataGen();
      break;
    case 85 : DAGtoCPDAGexample();
        break;

    default:
      std::cout << "Choice not valid" << std::endl;
    }

    std::cout << "Run another usecase ? (y/n) " << endl;

    std::cin >> rep;
  } while(rep == "y");
}

void displayPossibleChoices() {
  std::cout << "Select the usecase you want to run:" << std::endl;
    std::cout << "1 - createJuggler : Creates a simple bayesian network, "
                "prints it, save it in xml format, generate sample data as a csv file."
              << std::endl;

    std::cout << "2  - queryJuggler : Load and print a summary of the BN created"
              << "in 'createJuggler.cpp', in xml. Load the variables, et do queries."
              << std::endl;

    std::cout << "3  - learnParametersJuggler : Use the Network of 'createJuggler' to learn"
              << " new probabilistic distributions from csv data."
              << endl;

    std::cout << "4 - createSpam : Creates a simple bayesian network, "
                "prints it, save it in xml format, generate sample data as a csv file."
              << std::endl;

    std::cout << "5  - querySpam : Load and print a summary of the BN created"
              << "in 'createSpam.cpp', in xml. Load the variables, et do queries."
              << std::endl;

    std::cout << "6  - learnParametersSpam : Use the Network of 'createSpam' to learn"
              << " new probabilistic distributions from csv data."
              << endl;

    std::cout << "7  - createNoisyMax : " << endl;

    std::cout << "8  - queryNoisyMax : " << endl;

    std::cout << "9  - learnParametersNoisyMax : " << endl;

    std::cout << "10 - learnParamsNoisyMaxIncremental" << endl;

    std::cout << "11  - Dummy_GS_structureLearning" << endl;

    std::cout << "12 - createExam : create a simple bayesian network, prints it" << endl;

    std::cout << "13 - createMineSweeper : create a simple 4*1 minesweeper game bayesian network, prints it" << endl;

    std::cout << "14 - learnStructJuggler : learn the structure from the csv file generated in other usecase" << endl;

    std::cout << "15 - generateDBN : generate a csv file 1000*10" << endl;

    std::cout << "16 - learnStructDBN : learn using both GS and MMHC algorithm with new encapsulation" << endl;

    std::cout << "17 - createEyeColorGeneBN" << endl;

    std::cout << "18 - createEyeColorGeneBN_maxFn" << endl;

    std::cout << "19 - queryEyeColorGeneBN" << endl;

    std::cout << "20 - createDummyUsecase1" << endl;

    std::cout << "21 - createDummyUsecase2" << endl;

    std::cout << "22 - createDummyUsecase3" << endl;

    std::cout << "23 - LearnDummyUsecase1" << endl;

    std::cout << "24 - LearnDummyUsecaseIncremental" << endl;

    std::cout << "25 - TestingLearnIncremental" << endl;

    std::cout << "26 - non_stationnaireSystemExampleDummy" << endl;

    std::cout << "27 - LoadAndGenerateData from Asia.xdsl" << endl;

    std::cout << "28 - Asia_structureLearning with random init using Greedy Search (GS)" << endl;

    std::cout << "29 - Asia_GS_structureLearningWithMwstInit (GS)" << endl;

    std::cout << "30 - Asia_MWST_structureLearning (MWST)" << endl;

    std::cout << "31 - ASIA Structure Learning Comparison" << endl;

    std::cout << "32 - LoadHailFinderAndGenerateData" << endl;

    std::cout << "33 - LoadAlarmAndGenerateData" << endl;

    std::cout << "34 - Alarm_GS_structureLearning" << endl;

    std::cout << "35 - ExamenMarksBN" << endl;

    std::cout << "36 - ExamenMarksQuery" << endl;

    std::cout << "37 - ExamenMarksLearnParameters" << endl;

    std::cout << "38 - ExamenMarksLearnStruct" << endl;

    std::cout << "39 - createUniAdmissions" << endl;

    std::cout << "40 - queryUniAdmissions" << endl;

    std::cout << "41 - learnParametersUniAdmissions" << endl;

    std::cout << "42 - learnStrucUniAdmissions (GS)" << endl;

    std::cout << "43 - createMarks" << endl;

    std::cout << "44 - queryMarks" << endl;

    std::cout << "45 - learnParametersMarks" << endl;

    std::cout << "46 - learnStructMarks (MWST)" << endl;

    std::cout << "47 - createWeather" << endl;

    std::cout << "48 - learnParametersWeather" << endl;

    std::cout << "49 - learnParametersWeatherIncremental"<<endl;

    std::cout << "50 - Incremental FrequencyCounter test (Sufficient Statistics) IncrementalFC_test" << endl;

    std::cout << "51 - IMMHC" << endl;

    std::cout << "52 - MMHC with all the data combined" << endl;

    std::cout << "53 - create NoisyConditionals" <<endl;

    std::cout << "54 - test NoisyConditionals" <<endl;

    std::cout << "55 - learnParametersMultinet : pmClassifierMultinet" << endl;

    std::cout << "56 - learnParametersSingleBN : pmClassifierSingleBN" << endl;

    std::cout << "57 - learnParametersContraception : learnParameters for algo multi-net without class pmClassifier" << endl;

    std::cout << "58 - createContraception : create data with class" << endl;

    std::cout << "59 - createDataContraception : create data" << endl;

    std::cout << "60 - learnParametersData : learnParameters to check algo multi-net" << endl;

    std::cout << "61 - CreateNaiveTan : Creation naive structure for TAN algorithm" << endl;

    std::cout << "62 - LearnParametersNaiveTan : Creation naive structure for TAN algorithm" << endl;

    std::cout << "63 - learnParametersAbalone : classifieur naif" << endl;

    std::cout << "64 - createUnsupervisedSpam : Creation of Spam usecase with a latent class variable" << endl;

    std::cout << "65 - learnParametersUnsupervisedSpam : Learn parameters of the Bayesian Network with Spam as latent class " << endl;

    std::cout << "66 - learnParametersFWB : Learn the parameters of Financial well being usecase " << endl;

    std::cout << "67 - learnStructureFWB_GS : learn the structure of financial well being usecase with the GS algorithm" << endl;

    std::cout << "68 - learnStructureFWB_MWST : learn the structure of financial well being usecase with the MWST algorithm" << endl;

    std::cout << "69 - learnStructureFWB_MWSTinitGS : learn the structure of financial well being usecase with GS intilised with the MWST algorithm" << endl;

    std::cout << "70 - Create DBN with LO : Creates and save a DBN" << std::endl;

    std::cout << "71 - Query DBN with LO : Loads a DBN, learn parameters and serialise it" << std::endl;

    std::cout << "72 - create DBN identical : Creates and save a DBN where G0 = GT (minus the t+1 connections)" << std::endl;

    std::cout << "73 - create BKT : creates a BKT" << std::endl;

    std::cout << "74 - create_BN_and_xml : create a baysian network save it as xml and build another based on this xml" << endl;

    std::cout << "75 - usecase2_dataGen : generate data" << endl;

    std::cout << "76 - usecase2 : simple learnParameters exemple" << endl;

    std::cout << "77 - usecase2_incre : learmParameters incrementally" << endl;

    std::cout << "78 - usecase2_learnObj : learnParameters with learnObject" << endl;

    std::cout << "79 - usecase2_incre_learnObj : learnParam incrementally with learnObject" << endl;

    std::cout << "80 - usecase3_learnStruct : learn parameters and structure" << endl;

    std::cout << "81 - usecase3_learnStruct_constraint : learn parameters and structure" << endl;

    std::cout << "82 - usecase3_learnStructMWST : learn parameters and structure and c=show the cache" << endl;

    std::cout << "83 - create_DBN_and_xml : create a simple DBN and saves it as xml" << endl;

    std::cout << "84 - DataGen" << std::endl;

    std::cout << "85 - DAGtoCPDAGexample : transforms a DAG into a CPDAG" << std::endl;
}