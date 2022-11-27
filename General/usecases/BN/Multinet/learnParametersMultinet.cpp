/* ###################################################################
* Project : PILGRIM                                                  *
* Function : Usecase to use pmClassifierMultinet class               *
* Author : Soline PONSARD                                            *
* (c) Copyright juillet 2020                                         *
#################################################################### */

#include "learnParametersMultinet.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmClassifierMultinet.h>
#include <pilgrim/general/pmClassifier.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/cache.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>

using namespace std;
using namespace PILGRIM;

void learnParametersMultinet() {

  size_t line_skipped = 1;
  size_t nb_param = 6; // change if it's other dataset
  char delimiter = ';';

  char* data_file = "../../benchmarks/data/mammographic_masses.data"; // change if it's other dataset
  pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                         line_skipped,
                                         nb_param,
                                         delimiter);

    plVariablesConjunction vars = data->observed_variables();

    int classT = 0; // changed if it's other dataset
    plVariable classC = vars[classT];

    plVariable Class_new  = vars.get_variable_with_name("BI-RADS"); // change if it's other dataset
    plVariablesConjunction VarsWithoutClass = vars.remove(Class_new);

    pmClassifierMultinet classMulti(classC, VarsWithoutClass);

    classMulti.learnBN(data);

    cout << "\n \n // ============  Beggining Query ============= // \n\n";

    // change if it's other dataset
    plVariable A_new  = vars.get_variable_with_name("Age");
    plVariable B_new  = vars.get_variable_with_name("Shape");
    plVariable C_new  = vars.get_variable_with_name("Margin");
    plVariable D_new  = vars.get_variable_with_name("Density");
    plVariable E_new  = vars.get_variable_with_name("Severity");

    cout << "1st query" << endl;
    cout << "Age:71 Shape:4 Margin:4 Density:3 Severity:1" << endl;
    cout << endl;
    plValues evidence1_new(A_new^B_new^C_new^D_new^E_new);

    evidence1_new[A_new]  = 69;
    evidence1_new[B_new]  = 4;
    evidence1_new[C_new]  = 5;
    evidence1_new[D_new]  = 3;
    evidence1_new[E_new]  = 1;

    plDistribution jd = classMulti.query(evidence1_new);
    jd.tabulate();

    cout << "\n \n // ============  Finished Query ============= // \n\n";

    cout << "\n \n // ============  Beggining Confusion Matrix TAN ============= // \n\n";
    classMulti.confMatrix(data, true);
    cout << "\n \n // ============  Finished Confusion Matrix TAN ============= // \n\n";
  }
