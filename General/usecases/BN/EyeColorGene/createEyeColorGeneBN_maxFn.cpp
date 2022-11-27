#include "createEyeColorGeneBN.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace PILGRIM;

void createEyeColorGeneBN_maxFn()
{
    const double b = 0.5;

    // C source code that will be compiled dynamically during runtime.
    std::string my_code =
        "void my_max(double* output, unsigned int output_size,\n"
        " double* input, unsigned int input_size)\n"
        "{\n"
        " double* vectMax = output;\n"
        " double* vectProb = input;\n"
        "\n"
        " unsigned int n_probas; /* Number of values */\n"
        " double value_max=vectProb[0]; \n"
        " unsigned int i;\n"
        " \n"
        " n_probas = input_size; /* Get the number of values */\n"
        "\n"
        " for(i=0;i<n_probas;i++) /* Find the max value */\n"
        " if (vectProb[i]>value_max) value_max=vectProb[i];\n"
        " vectMax[0]=value_max; \n"
        "}\n";
    // Create the external function
    // const plExternalFunctionFromC max_fun(Y, U^V^W^L,my_code,"my_max");
    // const plCndDeterministic P_YgivenUVW(Y, U^V^W^L, max_fun);

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

    //Father Eye Color probability table using max function
    const plExternalFunctionFromC max_fun(FatherEyeColor, AlleleFather1 ^ AlleleFather2, my_code, "my_max");
    const plCndDeterministic FatherGivenFatherAllele1AndAllele2(FatherEyeColor, AlleleFather1 ^ AlleleFather2, max_fun);

    //MOTHER Eye Color probability table using max function
    const plExternalFunctionFromC max_fun2(MotherEyeColor, AlleleMother1 ^ AlleleMother2, my_code, "my_max");
    const plCndDeterministic MotherEyeColorGivenMotherAllele1AndAllele2(MotherEyeColor, AlleleMother1 ^ AlleleMother2, max_fun2);

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

    //Son EYE COLOR
    const plExternalFunctionFromC max_fun3(SonEyeColorVar, AlleleSon2 ^ AlleleSon1, my_code, "my_max");
    const plCndDeterministic SonEyeColorGivenMotherAllele1AndAllele2(SonEyeColorVar, AlleleSon2 ^ AlleleSon1, max_fun3);

    plJointDistribution jdEyeColor(AlleleFather1 ^ AlleleFather2 ^ AlleleMother1 ^ AlleleMother2 ^ AlleleSon1 ^ AlleleSon2 ^ SonEyeColorVar ^ FatherEyeColor ^ MotherEyeColor,
                                   P_AlleleFather1Var * P_AlleleFather2Var * P_AllelMother1Var * P_AllelMother2Var * P_AlleleSon1 * P_AlleleSon2 * SonEyeColorGivenMotherAllele1AndAllele2 * FatherGivenFatherAllele1AndAllele2 * MotherEyeColorGivenMotherAllele1AndAllele2);

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
    bnEyeColorGene.save_as_xml("../../benchmarks/networks/eyeGeneBN.xml", "eyeGeneBN");
    cout << "eyeGeneBN.xml generated in benchmarks/networks/ " << endl;
}