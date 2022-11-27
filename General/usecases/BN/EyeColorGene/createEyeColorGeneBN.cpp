#include "createEyeColorGeneBN.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace PILGRIM;

void createEyeColorGeneBN()
{
  const double b = 0.5;

  // Variables definitions
  // 0 for bleu et 1 pour marron
  plVariable SonEyeColorVar("SonEyeColorVar", PL_BINARY_TYPE);
  plVariable AlleleSon1("AlleleSon1", PL_BINARY_TYPE);
  plVariable AlleleSon2("AlleleSon2", PL_BINARY_TYPE);
  plVariable FatherEyeColor("FatherEyeColor", PL_BINARY_TYPE);
  plVariable AlleleFather1("AlleleFather1", PL_BINARY_TYPE);
  plVariable AlleleFather2("AlleleFather2", PL_BINARY_TYPE);
  plVariable MotherEyeColor("MotherEyeColor", PL_BINARY_TYPE);
  plVariable AlleleMother1("AlleleMother1", PL_BINARY_TYPE);
  plVariable AlleleMother2("AlleleMother2", PL_BINARY_TYPE);

  // Probability tables of AlleFather and AlleleMother
  plProbValue tableAlleleFather1Var[] = {b, 1 - b};
  plProbTable P_AlleleFather1Var(AlleleFather1, tableAlleleFather1Var);

  plProbValue tableAlleleFather2Var[] = {b, 1 - b};
  plProbTable P_AlleleFather2Var(AlleleFather2, tableAlleleFather2Var);

  plProbValue tableAlleleMother1Var[] = {b, 1 - b};
  plProbTable P_AllelMother1Var(AlleleMother1, tableAlleleMother1Var);

  plProbValue tableAlleleMother2Var[] = {b, 1 - b};
  plProbTable P_AllelMother2Var(AlleleMother2, tableAlleleMother2Var);

  // Probability tables of FatherEyeColors and MotherEyeColors
  plProbValue tableFatherEyeColor_knowing_AlleleFather1_AlleleFather2[] = {
      1, 0,
      0, 1,
      0, 1,
      0, 1};
  plDistributionTable P_FatherEyeColor(FatherEyeColor,
                                       AlleleFather1 ^ AlleleFather2,
                                       tableFatherEyeColor_knowing_AlleleFather1_AlleleFather2);

  plProbValue tableMotherEyeColor_knowing_AlleleMother1_AlleleMother2[] = {
      1, 0,
      0, 1,
      0, 1,
      0, 1};
  plDistributionTable P_MotherEyeColor(MotherEyeColor,
                                       AlleleMother1 ^ AlleleMother2,
                                       tableMotherEyeColor_knowing_AlleleMother1_AlleleMother2);

  plProbValue tableAlleleSon2_knowing_AlleleMother1_AlleleMother2[] = {
      1, 0,
      0.5, 0.5,
      0.5, 0.5,
      0, 1};
  plDistributionTable P_AlleleSon2(AlleleSon2,
                                   AlleleMother1 ^ AlleleMother2,
                                   tableAlleleSon2_knowing_AlleleMother1_AlleleMother2);

  plProbValue tableAlleleSon1_knowing_AlleleFather1_AlleleFather2[] = {
      1, 0,
      0.5, 0.5,
      0.5, 0.5,
      0, 1};
  plDistributionTable P_AlleleSon1(AlleleSon1,
                                   AlleleFather1 ^ AlleleFather2,
                                   tableAlleleSon1_knowing_AlleleFather1_AlleleFather2);

  // Probability table of RobotLacheBalle, which depends on BatterieFaible
  plProbValue tableSonEyeColorVar_knowing_AlleleSon1_AlleleSon2[] = {
      1, 0,
      0, 1,
      0, 1,
      0, 1};
  plDistributionTable P_SonEyeColorVar(SonEyeColorVar,
                                       AlleleSon1 ^ AlleleSon2,
                                       tableSonEyeColorVar_knowing_AlleleSon1_AlleleSon2);

  plJointDistribution jdEyeColor(AlleleFather1 ^ AlleleFather2 ^ AlleleMother1 ^ AlleleMother2 ^ AlleleSon1 ^ AlleleSon2 ^ SonEyeColorVar ^ FatherEyeColor ^ MotherEyeColor,
                                 P_AlleleFather1Var * P_AlleleFather2Var * P_AllelMother1Var * P_AllelMother2Var * P_AlleleSon1 * P_AlleleSon2 * P_SonEyeColorVar * P_FatherEyeColor * P_MotherEyeColor);

  // Creation of Bayesian network
  pmBayesianNetwork bnEyeColorGene(jdEyeColor);

  // Print Exam BN
  cout << "Eye Color BN " << endl;
  bnEyeColorGene.summary();
  cout << "\n\n";

  // Generate sample data as csv file
  bnEyeColorGene.generate_csvData("../../benchmarks/data/eyeGeneBN.data", 1000);
  cout << "eyeGeneBN.data generated in benchmarks/data/ " << endl;
  
  // Save the BNJuggler as xml
  bnEyeColorGene.save_as_xml("../../benchmarks/networks/eyeGeneBN.xml","eyeGeneBN");
  cout << "eyeGeneBN.xml generated in benchmarks/networks/ " << endl;
}