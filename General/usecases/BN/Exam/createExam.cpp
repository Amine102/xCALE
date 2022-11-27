#include "createExam.h"
#include <iostream>

using namespace std;
using namespace PILGRIM;

void createExam()
{
	// Create types

	// Create Variables
	plVariable Intelligent("Intelligent", PL_BINARY_TYPE);
	plVariable HardWorking("HardWorking", PL_BINARY_TYPE);
	plVariable GoodTestTaker("GoodTestTaker", PL_BINARY_TYPE);
	plVariable UnderstandsMaterial("UnderstandsMaterial", PL_BINARY_TYPE);
	plVariable HighExamScore("HighExamScore", PL_BINARY_TYPE);

    // Probability table of Intelligent
    plProbValue tableIntelligent[] = {0.3, 0.7};
    plProbTable P_Intelligent(  Intelligent, 
                                tableIntelligent);

    // Probability table HardWorking
    plProbValue tableHardWorking[] = {0.4, 0.6};
    plProbTable P_HardWorking(  HardWorking, 
                                tableHardWorking);

    // Probability table GoodTestTaker (from intelligent)
    plProbValue tableGoodTestTaker_knowing_Intelligent[] = {
        0.5, 0.5,
        0.2, 0.8
    };
    plDistributionTable P_GoodTestTaker(GoodTestTaker, 
                                        Intelligent, 
                                        tableGoodTestTaker_knowing_Intelligent);

    // Probability table UnderstandsMaterial (from Intelligent / HardWorking)
    plProbValue tableUnderstandsMaterial_knowing_Intelligent_HardWorking[] = {
        0.9, 0.1,
        0.5, 0.5,
        0.7, 0.3,
        0.1, 0.9
    };
    plDistributionTable P_UnderstandsMaterial(  UnderstandsMaterial, 
                                                Intelligent^HardWorking, 
                                                tableUnderstandsMaterial_knowing_Intelligent_HardWorking);

    // Probability table HighExamScore (from GoodTestTaker / UnderstandsMaterial)
    plProbValue tableHighExamScore_knowing_GoodTestTaker_and_UnderstandsMaterial[] = {
        0.7, 0.3,
        0.3, 0.7,
        0.5, 0.5,
        0.1, 0.9
    };
    plDistributionTable P_HighExamScore(HighExamScore, 
                                        GoodTestTaker^UnderstandsMaterial, 
                                        tableHighExamScore_knowing_GoodTestTaker_and_UnderstandsMaterial);

    // Joint distribution
    plJointDistribution jdExam( Intelligent^HardWorking^GoodTestTaker^UnderstandsMaterial^HighExamScore,
                                P_Intelligent*P_HardWorking*P_GoodTestTaker*P_UnderstandsMaterial*P_HighExamScore);

    // Creation of Bayesian network
    pmBayesianNetwork bnExam(jdExam);

    // Print Exam BN
    cout << "Exam Bayesian Network : " << endl;
    bnExam.summary();
    cout << "\n\n";

    //Exporting as csv file
    bnExam.generate_csvData("/deskop/Exam_csv.data", 1000);
}