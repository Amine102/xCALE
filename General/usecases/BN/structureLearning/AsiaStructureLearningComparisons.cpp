#include "AsiaStructureLearningComparisons.h"
#include "Asia_GS_structureLearningWithMwstInit.h"
#include "Asia_GS_structureLearningWithRandomInit.h"
#include "Asia_MWST_structureLearning.h"
#include <boost/graph/adjacency_list.hpp>

using namespace std;

void AsiaStructureLearningComparisons()
{
    //Asia GS without init BN nor GraphCPC
    cout << "\n================================AsiaGS Without Init BN================================\n"
         << endl;
    Asia_GS_structureLearningWithMwstInit();
    cout << "\n================================ENDED AsiaGS Without Init BN================================\n"
         << endl;

    cout << "\n================================AsiaGS with Random Init BN================================\n"
         << endl;
    //Asia with random initBN
    Asia_GS_structureLearningWithRandomInit();
    cout << "\n================================ENDED AsiaGS with Random Init BN================================\n"
         << endl;

    cout << "\n================================AsiaGS with MWST Init BN================================\n"
         << endl;
    //Asia with MWST initBN
    Asia_GS_structureLearningWithMwstInit();
    cout << "\n================================ENDED AsiaGS with MWST Init BN================================\n"
         << endl;

    cout << "\n================================Asia MWST================================\n"
         << endl;
    //Asia with MWST structure Learning
    Asia_MWST_structureLearning();
    cout << "\n================================ENDED Asia MWST================================\n"
         << endl;
}
