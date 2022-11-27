#include "createUniAdmissions.h"

using namespace std;
using namespace PILGRIM;

/**
 * \brief Creates a bayesian network based on an example from
 * "How To Implement Bayesian Networks In Python? – Bayesian Networks 
 * Explained With Examples" on Edureka by Zulaikha Lateef 
 * Prints it, save it in xml format, generate sample data as a csv file.
 */
void createUniAdmissions(){

  // Variable definitions
  
  
  plVariable ExamLevel("ExamLevel",PL_BINARY_TYPE);
  plVariable IQLevel("IQLevel",PL_BINARY_TYPE);
  plVariable Marks("Marks",PL_BINARY_TYPE);
  plVariable AptitudeScore("AptitudeScore",PL_BINARY_TYPE);
  plVariable Admission("Admission",PL_BINARY_TYPE);
  
  // Probability table of ExamLevel
  plProbValue tableExamLevel[]= {0.7, 0.3};
  plProbTable P_ExamLevel(ExamLevel, tableExamLevel);
  
  // Probability table of IQLevel
  plProbValue tableIQLevel[]={0.8, 0.2};
  plProbTable P_IQLevel(IQLevel, tableIQLevel);
  
  // Probability table of Marks, which depends on ExamLevel and IQLevel
  plProbValue tableMarks_knowing_ExamLevel_IQLevel[]={
      0.6, 0.4,
      0.9, 0.1,
      0.5, 0.5,
      0.8, 0.2
  };
  plDistributionTable P_Marks(Marks,
                              ExamLevel^IQLevel,
                              tableMarks_knowing_ExamLevel_IQLevel);
  
  // Probability table of AptitudeScore, which depends on IQLevel
  plProbValue tableAptitudeScore_knowing_IQLevel[] = {
      0.75, 0.25,
      0.4, 0.6
  };
  plDistributionTable P_AptitudeScore(AptitudeScore,
                                      IQLevel,
                                      tableAptitudeScore_knowing_IQLevel);
  
  // Probability table of Admission, which depends on Marks
  plProbValue tableAdmission_knowing_Marks[]={
      0.6, 0.4,
      0.9, 0.1
  };
  plDistributionTable P_Admission(Admission,
                                  Marks,
                                  tableAdmission_knowing_Marks);
  
  //Joint distribution
  plJointDistribution jdUniAdmissions(ExamLevel^IQLevel^Marks^AptitudeScore^Admission,
                                      P_ExamLevel*P_IQLevel*P_Marks*P_AptitudeScore*P_Admission);
                      
  // Creation of the bayesian network
  pmBayesianNetwork bnUniAdmissions(jdUniAdmissions);
  
  //Print of the bayesian network 
  cout << endl;
  cout << "UniAdmissions Bayesian Network summary : " << endl;
  bnUniAdmissions.summary();

  // Generate sample data as csv file
  bnUniAdmissions.generate_csvData("../../benchmarks/data/UniAdmissions_csv.data", 1000);
  cout << "UniAdmissions.data generated in benchmarks/data/ " << endl;

  // Save the BNUniAdmissions as xml
  bnUniAdmissions.save_as_xml("../../benchmarks/networks/jdBnUniAdmissions.xml","jdBnUniAdmissions");
  cout << "jdBnUniAdmissions.xml generated in benchmarks/networks/ " << endl;
}