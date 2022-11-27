#include <string>
#include <stdlib.h>
#include <algorithm> // std::any_of in BKTParams

#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <pilgrim/general/pm15dBN.h>

void createAndQueryBKT();

pmDynamicBayesianNetwork* createBKT();

void queryBKT(pmDynamicBayesianNetwork* DBN);
// Student 1
void queryBKTS1(pmDynamicBayesianNetwork* DBN);
// Student 2
void queryBKTS2(pmDynamicBayesianNetwork* DBN);
// Student 3
void queryBKTS3(pmDynamicBayesianNetwork* DBN);

// Helping funcs
void getEvidences(plValues& evidences_vector, std::vector<std::string> masteries_names,
                  std::vector<unsigned int> mastery_level);

void printBKTResult(std::vector<plComputableObjectList> results, std::vector<int> instants, 
                    std::vector<plVariablesConjunction> all_targets, bool formatted_for_spreadsheet=true);