/* ######################################################################
* Project : PILGRIM                                                     *
* Function : Usecase to use pmClassifierSingleBN class (TAN and NB)     *
* Author : Soline PONSARD                                               *
* (c) Copyright juillet 2020                                            *
####################################################################### */

#include "learnParametersSingleBN.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmClassifierSingleBN.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/cache.h>

using namespace std;
using namespace PILGRIM;

void learnParametersSingleBN() {

    size_t line_skipped = 1;
    size_t nb_param = 6; // change if it's other dataset
    char delimiter = ';';

    int classT = 0; // change if it's other dataset

    char* data_file = "../../benchmarks/data/mammographic_masses.data"; // change if it's other dataset
    pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                           line_skipped,
                                           nb_param,
                                           delimiter);

    plVariablesConjunction vars = data->observed_variables();
    plVariable classC = vars[classT];

    pmClassifierSingleBN classTAN(classC);
    pmClassifierSingleBN classNB(classC);

    classTAN.learnBN_TAN(data);
    classNB.learnBN_NB(data);

    cout << "\n \n // ============  Beggining Query TAN ============= // \n\n";

    // change if it's other dataset
    plVariable A_new  = vars.get_variable_with_name("Age");
    plVariable B_new  = vars.get_variable_with_name("Shape");
    plVariable C_new  = vars.get_variable_with_name("Margin");
    plVariable D_new  = vars.get_variable_with_name("Density");
    plVariable E_new  = vars.get_variable_with_name("Severity");

    plValues evidence1_new(A_new^B_new^C_new^D_new^E_new);

    cout << endl;
    cout << "1st query" << endl;
    cout << "Age:71 Shape:4 Margin:4 Density:3 Severity:1" << endl;
    cout << endl;
    evidence1_new[A_new]  = 69;
    evidence1_new[B_new]  = 4;
    evidence1_new[C_new]  = 5;
    evidence1_new[D_new]  = 3;
    evidence1_new[E_new]  = 1;

    plDistribution new_jd = classTAN.query(evidence1_new);
    new_jd.tabulate();

    cout << "\n \n // ============  Finished Query TAN ============= // \n\n";
    cout << "\n \n // ============  Beggining Query NB ============= // \n\n";

    // change if it's other dataset
    plVariable A_new_2  = vars.get_variable_with_name("Age");
    plVariable B_new_2  = vars.get_variable_with_name("Shape");
    plVariable C_new_2  = vars.get_variable_with_name("Margin");
    plVariable D_new_2  = vars.get_variable_with_name("Density");
    plVariable E_new_2  = vars.get_variable_with_name("Severity");

    plValues evidence2_new(A_new_2^B_new_2^C_new_2^D_new_2^E_new_2);

    cout << endl;
    cout << "2nd query" << endl;
    cout << "Age:71 Shape:4 Margin:4 Density:3 Severity:1" << endl;
    cout << endl;
    evidence2_new[A_new_2]  = 71;
    evidence2_new[B_new_2]  = 4;
    evidence2_new[C_new_2]  = 4;
    evidence2_new[D_new_2]  = 3;
    evidence2_new[E_new_2]  = 1;

    plDistribution new_jd2 = classNB.query(evidence2_new);
    new_jd2.tabulate();

    cout << "\n \n // ============  Finished Query NB ============= // \n\n";

    cout << "\n \n // ============  Beggining Confusion Matrix TAN ============= // \n\n";
    classTAN.confMatrix(data, true);
    cout << "\n \n // ============  Finished Confusion Matrix TAN ============= // \n\n";
    cout << "\n \n // ============  Beggining Confusion Matrix NB ============= // \n\n";
    classNB.confMatrix(data, true);
    cout << "\n \n // ============  Finished Confusion Matrix NB ============= // \n\n";
  }
