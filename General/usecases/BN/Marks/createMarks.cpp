#include "createMarks.h"
#include <vector>

using namespace std;
using namespace PILGRIM;
/**
 * \brief Creates a bayesian network
 * prints it, save it in xml format, generate sample data as a csv file.
 */
void createMarks(){

  vector<string> examType_labels;
  examType_labels.push_back("Test");
  examType_labels.push_back("HomeworkAssignment");

  vector<string> examLevel_labels;
  examLevel_labels.push_back("Easy");
  examLevel_labels.push_back("Difficult");

  vector<string> livingEnvironment_labels;
  livingEnvironment_labels.push_back("Noisy");
  livingEnvironment_labels.push_back("Quiet");

  vector<string> sleepDuration_labels;
  sleepDuration_labels.push_back("-7H");
  sleepDuration_labels.push_back("+7H"); 

  vector<string> time_labels;
  time_labels.push_back("0H-2H");
  time_labels.push_back("2H-10H");
  time_labels.push_back("10H+");

  // Variable definitions
  plVariable ExamType("ExamType",plLabelType(examType_labels));
  plVariable LivingEnvironment("LivingEnvironment",plLabelType(livingEnvironment_labels));
  plVariable StudentJob("StudentJob",PL_BINARY_TYPE);
  plVariable ExamLevel("ExamLevel", plLabelType(examLevel_labels));
  plVariable Sleep("Sleep",plLabelType(sleepDuration_labels));
  plVariable AttentiveInClass("AttentiveInClass", PL_BINARY_TYPE);
  plVariable StudentLevel("StudentLevel",PL_BINARY_TYPE);
  plVariable StudyTime("StudyTime",plLabelType(time_labels));
  plVariable Mark("Mark",PL_BINARY_TYPE);

  // Probability table of ExamType
  plProbValue tableExamType[]= {0.75, 0.25};
  plProbTable P_ExamType(ExamType,tableExamType);
                          
  // Probability table of LivingEnvironment
  plProbValue tableLivingEnvironment[]={0.35, 0.65};
  plProbTable P_LivingEnvironment(LivingEnvironment,tableLivingEnvironment);

  // Probability table of StudentJob
  plProbValue tableStudentJob[]={0.23, 0.77};
  plProbTable P_StudentJob(StudentJob,tableStudentJob);

  // Probability table of ExamLevel, which depends on ExamType
  plProbValue tableExamLevel_knowing_ExamType[]= {
    0.35, 0.65,
    0.70, 0.30
  };
  plDistributionTable P_ExamLevel(ExamLevel,
                                  ExamType,
                                  tableExamLevel_knowing_ExamType);


  // Probability table of Sleep, which depends on LivingEnvironment
  plProbValue tableSleep_knowing_LivingEnvironment[]={
    0.65, 0.35,
    0.50, 0.50
  };
  plDistributionTable P_Sleep(Sleep,
                              LivingEnvironment,
                              tableSleep_knowing_LivingEnvironment);

  // Probability table of AttentiveInClass, which depends on Sleep
  plProbValue tableAttentiveInClass_knowing_Sleep[]={
    0.7, 0.3,
    0.3, 0.7
  };
  plDistributionTable P_AttentiveInClass(AttentiveInClass,
                                        Sleep,
                                        tableAttentiveInClass_knowing_Sleep);

  // Probability table of StudentLevel, which depends on AttentiveInClass
  plProbValue tableStudentLevel_knowing_AttentiveInClass[]={
    0.90, 0.10,
    0.58, 0.42
  };
  plDistributionTable P_StudentLevel(StudentLevel,
                                    AttentiveInClass,
                                    tableStudentLevel_knowing_AttentiveInClass);

  // Probability table of StudyTime, which depends on LivingEnvironment and StudentJob
  plProbValue tableStudyTime_knowing_LivingEnvironment_StudentJob[]={
    0.45, 0.52, 0.03,
    0.25, 0.61, 0.14,
    0.28, 0.64, 0.08,
    0.12, 0.68, 0.20
  };
  plDistributionTable P_StudyTime(StudyTime,
                                  LivingEnvironment^StudentJob,
                                  tableStudyTime_knowing_LivingEnvironment_StudentJob);

  //Probability table of Mark, wich depends on StudentLevel, ExamLevel and StudyTime
  plProbValue tableMark_knowing_StudentLevel_ExamLevel_StudyTime[]={
    0.82, 0.18,
    0.64, 0.36,
    0.52, 0.48,
    0.99, 0.01,
    0.84, 0.16,
    0.73, 0.27,
    0.32, 0.68,
    0.14, 0.86,
    0.02, 0.98,
    0.55, 0.45,
    0.36, 0.62,
    0.29, 0.71,
  };
  plDistributionTable P_Mark(Mark,
                            StudentLevel^ExamLevel^StudyTime,
                            tableMark_knowing_StudentLevel_ExamLevel_StudyTime);


  //Joint distribution
  plJointDistribution  jdMarks(ExamType^LivingEnvironment^StudentJob^ExamLevel^Sleep^AttentiveInClass^StudentLevel^StudyTime^Mark,
                              P_ExamType*P_LivingEnvironment*P_StudentJob*P_ExamLevel*P_Sleep*P_AttentiveInClass*P_StudentLevel*P_StudyTime*P_Mark);

  // Creation of the bayesian network
  pmBayesianNetwork bnMarks(jdMarks);

  //Print of the bayesian network 
  cout << endl;
  cout << "Marks Bayesian Network summary : " << endl;
  bnMarks.summary();

  // Generate sample data as csv file
  bnMarks.generate_csvData("../../benchmarks/data/Marks_csv.data", 1000);
  cout << "Marks.data generated in benchmarks/data/ " << endl;

  // Save the bnMarks as xml
  bnMarks.save_as_xml("../../benchmarks/networks/jdBnMarks.xml","jdBnMarks");
  cout << "jdBnMarks.xml generated in benchmarks/networks/ " << endl;
}