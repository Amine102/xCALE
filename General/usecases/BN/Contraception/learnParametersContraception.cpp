/* ###################################################################
* Project :                                                          *
* Function : Multinet algo to learn structure and query              *
* cf. multinet usecase to use pmClassifier class                     *
* Author : Soline PONSARD                                            *
* (c) Copyright juin 2020                                                *
#################################################################### */

#include "learnParametersContraception.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/cache.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>

using namespace std;
using namespace PILGRIM;

void learnParametersContraception() {

    size_t line_skipped = 1;
    size_t nb_param = 6; // A changer si autre dataset
    size_t nb_param_inc = nb_param-1;
    char delimiter = ';';
    pmScoreValueType priorWeight = 1;

    plError error;
    error.ignore_this_message(35,true);

    char* data_file = "../../benchmarks/data/mammographic_masses.data"; //PARAM
    pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                           line_skipped,
                                           nb_param,
                                           delimiter);

    plCSVFileDataDescriptor* plDataDescriptor = data->get_data_descriptor();
    int nbRow = plDataDescriptor->get_n_records();

    plVariablesConjunction vars = data->observed_variables();

    plVariable classCA = vars[0]; //PARAM
    int classC = vars.get_variable_position(classCA);

    plType type = vars[classC].get_type();
    vector<string> vecVar =  type.get_values_as_strings();

    plVariable Class_new  = vars.get_variable_with_name("BI-RADS"); //A changer si autre dataset
    plVariablesConjunction VarsWithoutClass = vars.remove(Class_new);

    map<string,double> Cproba;
    double total = 0.0;

    for (string str : vecVar) {

        double totalPart = 0.0;
        double totalPart2 = 0.0;

        const char* new_str1 = str.c_str();
        char* new_str2 = "../../benchmarks/data/dataclass_";
        char* new_str3 = ".data";

        int size1 = strlen(new_str1);
        int size2 = strlen(new_str2);
        int size3 = strlen(new_str3);

        char file[size1+size2+size3];
        file[0]='\0';

        strcat(file,new_str2);
        strcat(file,new_str1);
        strcat(file,new_str3);


        char* new_data_file = file;
        cout << "new_data_file : " << new_data_file << endl;

        pmCSVDataSet* new_data = new pmCSVDataSet( new_data_file,
                                                   line_skipped,
                                                   nb_param_inc,
                                                   delimiter,
                                                   VarsWithoutClass);

        plCSVFileDataDescriptor* new_plDataDescriptor = new_data->get_data_descriptor();
        int new_nbRow = new_plDataDescriptor->get_n_records();

        double proba = double(new_nbRow)/nbRow;
        cout << "P( C = "+str+" ) : " << proba << endl;

        plVariablesConjunction new_vars = new_data->observed_variables();;

        cout << "\n \n // ============  Beggining Trading from Robankhood Structural Learning ============= // \n\n";

        string name_xml ="../../benchmarks/networks/lpBnClassificationEAP_"+str+".xml";

        // set up frequency counter
        pmFrequencyCounter *fc = new pmFrequencyCounter(new_data->get_data_descriptor(),
        new_vars);
        fc->computeFrequencies();

        // Use cache for faster computation
        pmCache *cache = new PILGRIM::pmCache(1000000000);

        //create score and algorithm
        pmBayesianNetwork bn_empty(new_vars);

        pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
        pmAlgoMWST <rowDataType> algo_MWST(new_data, pScore);

        // compute BN MWST
        pmBayesianNetwork *bn_learned_MWST = new pmBayesianNetwork(algo_MWST.getVariables());
        algo_MWST.run(bn_learned_MWST);
        bn_learned_MWST->initComputableObjectList(true, true);

        pmGraph bn_learned_Graph = bn_learned_MWST->get_graph();
        plBayesianNetwork pl_bn(bn_learned_MWST->getJointDistribution(),"BN classification "+str+" MWST");
        pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn__Classification_"+str+"_Trading_MWST.dot");

        bn_learned_MWST->setLearnObjectEAP(priorWeight);
        bn_learned_MWST->learnParameters(new_data);

        // bn_learned_MWST->summary();

        cout << "\n \n // ============  Finished Trading from Robankhood Structural Learning ============= // \n\n";
        cout << "\n \n // ============  Beggining Query ============= // \n\n";

        plVariablesConjunction Variables = bn_learned_MWST->getVariables();

        plJointDistribution jd = bn_learned_MWST->getJointDistribution();

        plVariable A_new  = Variables.get_variable_with_name("Age");
        plVariable B_new  = Variables.get_variable_with_name("Shape");
        plVariable C_new  = Variables.get_variable_with_name("Margin");
        plVariable D_new  = Variables.get_variable_with_name("Density");
        plVariable E_new  = Variables.get_variable_with_name("Severity");

        plValues evidence1_new(A_new^B_new^C_new^D_new^E_new);

        evidence1_new[A_new]  = 69;
        evidence1_new[B_new]  = 4;
        evidence1_new[C_new]  = 5;
        evidence1_new[D_new]  = 3;
        evidence1_new[E_new]  = 1;

        cout << "1st query" << endl;
        plDistribution pld_new = jd.ask(VarsWithoutClass);
        plProbValue val2 = jd.compute_log(evidence1_new);
        plProbValue val = jd.compute(evidence1_new);


        totalPart = val*proba;
        totalPart2 = val2+log(proba);
        total = total + totalPart;
        Cproba[str] = totalPart2;

        cout << "\n \n // ============  Finished Query ============= // \n\n";

      }

      //Afficher la map
      pair<string,double> maxT;

      // cout << "Probabilities" << endl;
      map<string, double>::iterator p;
      double max = -100000;
      for(p = Cproba.begin(); p != Cproba.end(); p++) {
        double tot = double(p->second)-log(total);
        cout << "P (C = " << p->first << " | Xi) = "<< exp(tot) << endl;
        if (max<tot) {
          max = tot;
          maxT.first = p->first;
          maxT.second = max;
        }
      }
        cout << "Best Probability : P (C = " << maxT.first << " | Xi) = "<< exp(max) << endl;
    }
