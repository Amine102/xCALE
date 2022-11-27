#include "IncrementalFC_test.h"
#include <string>
#include <iostream>
#include <pilgrim/general/pmSlidingWindow.h>

using namespace std;
using namespace PILGRIM;

void IncrementalFC_test()
{
     cout << "====================Test 5====================" << endl;
     cout << "\nLoading Data" << endl;
     char *data_file = "../../benchmarks/data/Asia1.data";
     pmCSVDataSet *data = new pmCSVDataSet(data_file);
     plVariablesConjunction vars = data->observed_variables();

     float forgettingCoefficient = 0.7;

     cout << "Before creating the FC, pmSlidingWindow::N is ";
     cout << pmSlidingWindow::N << endl;
     cout << "Before creating the FC, pmSlidingWindow::N_old is ";
     cout << pmSlidingWindow::N_old << endl;
     //===========================================================================================================
     cout << "\n\n==========================================================================Creating a frequency Counter with Data 1 and calling FrequencyCount()==========================================================================\n\n"
          << endl;
     cout << "\n\nCreating Frequency Counter" << endl;
     pmFrequencyCounter fc1(data, vars, true, forgettingCoefficient, true);
     vector<plFloat> freq;

     cout << "After creating the FC with data1 pmSlidingWindow::N is ";
     cout << pmSlidingWindow::N << endl;
     cout << "After creating the FC with data1 pmSlidingWindow::N_old is ";
     cout << pmSlidingWindow::N_old << endl;

     //===========================================================================================================
     cout << "\n\n========Searched variables contain Variable 0 and Variable 1========" << endl;
     vector<unsigned int> searchedVars;
     searchedVars.push_back(0);
     searchedVars.push_back(1);

     cout << "\n\nCalling frequencyCount\n\n"
          << endl;
     fc1.frequencyCount(searchedVars, freq);

     fc1.printSW_Summary();
     //===========================================================================================================
     cout << "\n\n========Searched variables contain Variable 0 and Variable 3========" << endl;
     searchedVars.clear();
     searchedVars.push_back(0);
     searchedVars.push_back(3);

     cout << "\n\nCalling frequencyCount\n\n"
          << endl;
     fc1.frequencyCount(searchedVars, freq);

     fc1.printSW_Summary();
     //===========================================================================================================
     cout << "\n\n========Searched variables contain Variable 0 and Variable 2========" << endl;
     searchedVars.clear();
     searchedVars.push_back(0);
     searchedVars.push_back(2);

     cout << "\n\nCalling frequencyCount\n\n"
          << endl;
     fc1.frequencyCount(searchedVars, freq);

     fc1.printSW_Summary();

     cout << "\n\nFinished Using the data 1 with FC1\n\n"
          << endl;
     //===========================================================================================================
     cout << "\n\n==========================================================================Updating Cache and adding a previousBN==========================================================================\n\n"
          << endl;
     cout << "\n\nPreviousBN initialization" << endl;
     pmBayesianNetwork bn_temp("../../benchmarks/networks/asia_learnedStructureAlgoMMHC_MWSTinitBN1.xml", "asiaMMHC");
     pmBayesianNetwork previousBN(data->observed_variables());
     previousBN.copy_structure(bn_temp);

     fc1.updateSW(previousBN);
     //===========================================================================================================
     cout << "\n\n==========================================================================Creating a frequency Counter with Data 2==========================================================================\n\n"
          << endl;
     data_file = "../../benchmarks/data/Asia2.data";
     data = new pmCSVDataSet(data_file);

     pmFrequencyCounter fc2(data, vars, true, forgettingCoefficient, true);
     vector<plFloat> freq2;

     cout << "After creating the FC2 with data2 pmSlidingWindow::N is ";
     cout << pmSlidingWindow::N << endl;
     cout << "After creating the FC2 with data2 pmSlidingWindow::N_old is ";
     cout << pmSlidingWindow::N_old << endl;
     //===========================================================================================================

     cout << "\n\n========Searched variables contain Variable 0 and Variable 1========" << endl;
     searchedVars.clear();
     searchedVars.push_back(0);
     searchedVars.push_back(1);

     cout << "\n\nCalling frequencyCount\n\n"
          << endl;
     fc2.frequencyCount(searchedVars, freq2);

     fc2.printSW_Summary();
     //===========================================================================================================

     cout << "\n\n========Searched variables contain Variable 0 and Variable 2========" << endl;
     searchedVars.clear();
     searchedVars.push_back(0);
     searchedVars.push_back(2);

     cout << "\n\nCalling frequencyCount\n\n"
          << endl;
     fc2.frequencyCount(searchedVars, freq2);

     cout << "\n\nCache after the frequency Count" << endl;
     fc2.printSW_Summary();
     //===========================================================================================================

     cout << "\n\n========Searched variables contain Variable 0 and Variable 1 and Variable 2========" << endl;
     searchedVars.clear();
     searchedVars.push_back(0);
     searchedVars.push_back(1);
     searchedVars.push_back(2);

     cout << "\n\nCalling frequencyCount\n\n"
          << endl;
     fc2.frequencyCount(searchedVars, freq2);

     fc2.printSW_Summary();
     //===========================================================================================================

     cout << "\n\n==========================================================================Updating Cache and adding a previousBN==========================================================================\n\n"
          << endl;
          
     fc2.updateSW(previousBN);
}