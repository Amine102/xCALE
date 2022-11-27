/* STILL UNDER DEVELOPMENT */
/* INCREMENTAL LEARNING USING INCOMPLETE DATA*/

#include "learnParamsNoisyMaxIncremental.h"
#include <string>
#include <stdlib.h>
#include "Helper_Functions.h"
#include "../DummyUseCase/HelperFunctions.h"

using namespace std;
using namespace PILGRIM;

void learnParamsNoisyMaxIncremental()
{

    // Learning Non-Incrementally to observe the difference later
    // learnParamsIncompleteData();
    // ---------------------------------------------------------------------------

    /*Variables initialisation*/
    pmBayesianNetwork bnNoisyMax;
    size_t window_size = 1;
    size_t numberOfColumns = 4;
    double forgettingFactor = 0.7;
    std::vector<plLearnObject *> learn_Object_List;
    string splitted_file;
    string path_toIncompleteData;
    int numberOfSplits = 10;
    char *data_file;
    pmCSVDataSet *data;

    pmBayesianNetwork bnNoisyMaxTemp("../../benchmarks/networks/jdNoisyMax.xml", "jdNoisyMax");
    bnNoisyMax = pmBayesianNetwork(bnNoisyMaxTemp, false);
    // learn_Object_List = fillLearnObjectList(vars);
    //----------------------------------------------------------------------------------------------------------------
    plVariablesConjunction variables = bnNoisyMax.getVariables();

    plVariable A = variables.get_variable_with_name("A");
    plVariable B = variables.get_variable_with_name("B");
    plVariable C = variables.get_variable_with_name("C");
    plVariable L = variables.get_variable_with_name("L");
    plVariable U = variables.get_variable_with_name("U");
    plVariable V = variables.get_variable_with_name("V");
    plVariable W = variables.get_variable_with_name("W");
    plVariable Y = variables.get_variable_with_name("Y");

    std::vector<plLearnObject *> learnObjectList;
    learnObjectList.push_back(new plLearnHistogram(A));
    learnObjectList.push_back(new plLearnHistogram(B));
    learnObjectList.push_back(new plLearnHistogram(C));
    learnObjectList.push_back(new plLearnHistogram(L));
    // ---------------------------------------------------------------------------
    plDeterministic P_U0(U, get_Label_Index(U, "1-ok")); //takes a plValue as a second parameter
    plLearnDistributionTable PL_U(U, A);

    PL_U.push(plLearnFrozenDistribution(P_U0), get_Label_Index(A, "1-ok"));
    PL_U.push(plLearnHistogram(U), get_Label_Index(A, "3-ko"));
    PL_U.push(plLearnHistogram(U), get_Label_Index(A, "2-deg"));

    learnObjectList.push_back(&PL_U);
    // ---------------------------------------------------------------------------

    plDeterministic P_V0(V, get_Label_Index(V, "1-ok")); //takes a plValue as a second parameter
    plLearnDistributionTable PL_V(V, B);
    PL_V.push(plLearnFrozenDistribution(P_V0), get_Label_Index(B, "1-ok"));
    PL_V.push(plLearnHistogram(V), get_Label_Index(B, "3-ko"));
    PL_V.push(plLearnHistogram(V), get_Label_Index(B, "2-deg"));

    learnObjectList.push_back(&PL_V);

    plDeterministic P_W0(W, get_Label_Index(W, "1-ok")); //takes a plValue as a second parameter
    plLearnDistributionTable PL_W(W, C);
    PL_W.push(plLearnFrozenDistribution(P_W0), get_Label_Index(C, "1-ok"));
    PL_W.push(plLearnHistogram(W), get_Label_Index(C, "3-ko"));
    PL_W.push(plLearnHistogram(W), get_Label_Index(C, "2-deg"));

    learnObjectList.push_back(&PL_W);

    // C source code function :
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

    cout << "starting creating dynamic code" << endl;
    const plExternalFunctionFromC max_fun(Y, U ^ V ^ W ^ L, my_code, "my_max");
    const plCndDeterministic P_YgivenUVWL(Y, U ^ V ^ W ^ L, max_fun);

    learnObjectList.push_back(new plLearnFrozenDistribution(P_YgivenUVWL));
    // --------------------------------------------------------------------------------------------------------------------

    bnNoisyMax.setLearnObject(learnObjectList);

    //Bash Command to remove the unnecessary columns
    string str = "cut -f -3,8 -d';' ../../benchmarks/data/NoisyMax+latentVars_csv.data > ../../benchmarks/data/NoisyMax_csv.data";
    const char *command = str.c_str();
    ::system(command);

    path_toIncompleteData = "../../benchmarks/data/NoisyMax_csv.data";
    splitData(path_toIncompleteData, numberOfSplits);

    splitted_file = path_toIncompleteData + "0" + ".data";
    data_file = strcpy(new char[splitted_file.length() + 1], splitted_file.c_str());
    data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', A ^ B ^ C ^ Y);

    //Non Incremental Learning for the file
    cout << "Learning Non-incrementally for the FIRST FILE" << endl;
    bnNoisyMax.learnParameters(data);
    exportVectorOfProba(bnNoisyMax.getComputableObjectList(), "P(A)", 0);
    cout<<"after exportVectorOfProba"<<endl;
    for (int i = 1; i < numberOfSplits; i++)
    {
        splitted_file = path_toIncompleteData + to_string(i) + ".data";
        data_file = strcpy(new char[splitted_file.length() + 1], splitted_file.c_str());
        data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', A ^ B ^ C ^ Y);

        bnNoisyMax.setForgettingFactor(forgettingFactor);
        cout << "learnParametersIncremental" << endl;
        bnNoisyMax.learnParameters(data);
        exportVectorOfProba(bnNoisyMax.getComputableObjectList(), "P(A)", i);
    }
    bnNoisyMax.summary();
    bnNoisyMax.save_as_xml("../../benchmarks/networks/LearnedIncrementalNoisyMax.xml", "jdNoisyMaxLearned");
}